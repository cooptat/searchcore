#ifndef _SEARCH_INDEXER_H_
#define _SEARCH_INDEXER_H_

#include "core/search_defs.h"
#include "core/Index.h"
#include "core/DocRepr.h"
#include "core/Dict.h"

namespace searchcell {

template<
	class Index,
	class Doc,
	class DocCompiler,
	
	template<class T> class Alloc
>
class IndexerT {

	typedef IndexRowT<Alloc> IndexRow;
	typedef typename IndexRow::Ptr IndexRowPtr;
	typedef typename Index::Ptr IndexPtr;
	typedef typename Dict::Ptr DictPtr;
	typedef typename DocCompiler::Ptr DocCompilerPtr;

	//typedef typename Doc::Ptr DocPtr;

	DictPtr m_dict;
	IndexPtr m_index;
	DocCompilerPtr m_doc_compiler;

public:
	
	typedef std::shared_ptr<IndexerT> Ptr;

	IndexerT(DictPtr _dict, IndexPtr _index, DocCompilerPtr _doc_compiler):
		m_dict(_dict),
		m_index(_index),
		m_doc_compiler(_doc_compiler) {
	}
	
	bool isIndexSorted() const {
		return m_index->isSorted();
	}
	
	void sort() {
		
		m_index->sort();
	}

	void indexDoc(DocId _docid, const Doc &_doc) {
      
      DocRepr doc_repr;
      m_doc_compiler->compile(_doc, doc_repr);

      _indexDocRepr(_docid, doc_repr);
	}

private:

	void _indexDocRepr(DocId _docid, const DocRepr &_textrepr) {

		for (auto t : _textrepr.tokens) {
			
			auto rowid = RowId(t.token, t.exact);

			auto it = m_index->find(rowid);
			if (it != m_index->end()) {
				
				(*it).second->add(_docid);	
				continue;
			}

			typename IndexRow::OccursVector occurs;
			occurs.push_back( _docid );

			auto row_p = Alloc<IndexRow>().allocate(1);
			Alloc<IndexRow>().construct( row_p, isIndexSorted(), occurs );
			IndexRowPtr row ( row_p );

			m_index->put(std::make_pair( rowid, row));
		}
	}
};

};

#endif
