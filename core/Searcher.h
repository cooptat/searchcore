#ifndef _SEARCH_SEARCHER_H_
#define _SEARCH_SEARCHER_H_

#include "search_defs.h"
#include "Index.h"
#include "DocRepr.h"
#include "QueryRepr.h"

#include <iostream>

namespace searchcell {

template <
	class Index,
	class Query,
	class QueryCompiler
>
class SearcherT {
public:

  typedef typename Dict::Ptr DictPtr;
  typedef typename Index::Ptr IndexPtr;
	typedef typename QueryCompiler::Ptr QueryCompilerPtr;
  typedef std::shared_ptr<SearcherT> Ptr;

private:

	DictPtr m_dict;
	IndexPtr m_index;
	QueryCompilerPtr m_query_compiler;

public:

	SearcherT(DictPtr _dict, IndexPtr _index, QueryCompilerPtr _query_compiler):
		m_dict(_dict),
		m_index(_index),
		m_query_compiler(_query_compiler) {
	}
	
	void query(const Query &_query, std::vector<DocId> &_docs) {
		
		QueryRepr query_repr;
		m_query_compiler->compile(_query, query_repr);
		_do_query(query_repr, _docs);
	}
		
private:
	
	void _do_query(const QueryRepr &_query, std::vector<DocId> &_docs) {
		
		if (_query.size() == 0)
			return;

		auto index_end = m_index->end();
		
		typename Index::IndexRow final_row(true);
		for (auto i = 0; i<_query.tokens.size(); ++i) {
			
			if (i == 0) {
				
				typename Index::iterator it = m_index->end();
				
				//std::cout << "Quering " << RowId(query.tokens[i].word, query.tokens[i].need_exact).dump() << std::endl;

				it = m_index->find( RowId(_query.tokens[i].word, true));
				
				if (!_query.tokens[i].need_exact && it == index_end) {
				
					it = m_index->find( RowId(_query.tokens[i].stem, false));
					if (it == index_end)
						it = m_index->find( RowId(_query.tokens[i].stem, true));
				}

				if (it != index_end) {
					
					//std::cout << "Searching assigning row\n";
					final_row = *((*it).second.get());
					continue;
				}
				
				break;
			}
			
			typename Index::iterator it = m_index->end();
			
			it = m_index->find( RowId(_query.tokens[i].word, true));
				
			if (!_query.tokens[i].need_exact && it == index_end) {
			
				it = m_index->find( RowId(_query.tokens[i].stem, false));
				if (it == index_end)
					it = m_index->find( RowId(_query.tokens[i].stem, true));
			}
			
			if (it == index_end) {
				_docs.clear();
				return;
			}
			
			final_row.intersect( *((*it).second.get()) );
		}
		
		final_row.getDocIds(_docs);
	}
};

};

#endif
