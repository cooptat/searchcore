#ifndef CPT_SEARCH_CELL_H_
#define CPT_SEARCH_CELL_H_

#include "core/search_defs.h"
#include "core/MemIndex.h"
#include "core/Indexer.h"
#include "core/Searcher.h"
#include "core/Dict.h"

#include <unordered_map>

#include <boost/filesystem.hpp>

/*

	Query memory-only

	Load on start
		Dict
		Index

	Dump with fork & mmap
	
	No deleting

*/

namespace searchcell {

template <

    class Doc,
    class DocLoader,
      // .next(std::vector<Doc>&, int _max) -> bool
      // .end() -> bool
      // .reset()

    class DocCompiler,
      // .compile(const Doc&, DocRepr&) 

    class Query,
	class QueryCompiler,
	  // .compile(const Query&, QueryRepr&) 
	
	class Index,
	class Searcher,
	
	template<class T> class Alloc
>
class SearchCellT {
public:

	/*
		Dependencies:
			
			TokenId == uint32_t

			Dict
			TextRepr
			Index < DocId >
				IndexRow < DocId >
					IndexOccur < DocId >
			QueryToken
	*/
	
	//typedef typename Doc::Ptr DocPtr;

	typedef SearchCellT<Doc, DocLoader, DocCompiler, Query, QueryCompiler, Index, Searcher, Alloc> SearchCell;
	typedef boost::shared_ptr< SearchCell > Ptr;
    
	typedef std::shared_ptr< DocLoader > DocLoaderPtr;
	typedef std::shared_ptr< DocCompiler > DocCompilerPtr;
	typedef std::shared_ptr< QueryCompiler > QueryCompilerPtr;

	typedef typename Dict::Ptr DictPtr;
	typedef typename Index::Ptr IndexPtr;
	
	typedef IndexerT< Index, Doc, DocCompiler, Alloc > Indexer;
	typedef typename Indexer::Ptr IndexerPtr;


	typedef typename Searcher::Ptr SearcherPtr;

	struct Params {
		std::string dump_dir;
		DictPtr dict;
		DocLoaderPtr loader;
		DocCompilerPtr doc_compiler;
		QueryCompilerPtr query_compiler;
	};

	const int m_load_batch_size = 32768;

private:

	Params m_params;

	DocLoaderPtr m_loader;
	DocCompilerPtr m_doc_compiler;
	QueryCompilerPtr m_query_compiler;

	DictPtr m_dict;
	IndexPtr m_index;


	SearcherPtr m_searcher;

	std::string m_dump_dir;
	std::string m_dict_dir;
	std::string m_index_fn;
	
	bool m_ready = false;

public:

	SearchCellT(const Params &_params):
	  m_params(_params),
	  m_dict(_params.dict),
	  m_loader(_params.loader),
	  m_doc_compiler(_params.doc_compiler),
	  m_query_compiler(_params.query_compiler),
	  m_dump_dir(_params.dump_dir),
	  m_dict_dir( m_dump_dir + "/dict/" ),
	  m_index_fn( m_dump_dir + "/index" ) {

	  //m_dict.reset(new Dict( m_dict_dir ));
	  m_ready = false;
	}

	//void forkDump();
	
	void loadIndex() {
	  
  	  m_ready = false;
  	  _loadIndex();
  	  m_ready = true;
	}
	
	void reindex() {

	  m_ready = false;
	  _reindex();
	  m_ready = true;
	}

	void query(const Query &_usr_query, std::vector<DocId> &_res) {

	  if (!m_ready)
	  	throw std::logic_error("SearchCell::query SearchCell not ready: load or rebuild index");

	  _query(_usr_query, _res);
	}

private:

	void _loadIndex() {
	  
      // check if directory exists and index loads else throw
  	  auto index_p = Alloc<Index>().allocate(1); 
  	  Alloc<Index>().construct( index_p, true ); // sorted
      m_index.reset( index_p );
	  
	  std::cout << "SearchCell::_loadIndex\n";
	  m_index->load( m_index_fn );
	  std::cout << "SearchCell::_loadIndex done\n";
	  
	  // Searcher
	  m_searcher.reset(new Searcher( m_dict, m_index, m_query_compiler ));
	}

	void _reindex() {

	  // create directory
      boost::filesystem::remove_all( m_index_fn );
      //boost::filesystem::create_directory( m_index_dir );
	  
	  m_dict->clear();

      // Index	  
	  auto index_p = Alloc<Index>().allocate(1); 
	  Alloc<Index>().construct( index_p, false ); // not sorted
      m_index.reset( index_p );

	  // Indexer
	  IndexerPtr indexer(new Indexer( m_dict, m_index, m_doc_compiler ));

      std::vector< std::pair< DocId, Doc > > docs;

	  std::cout << "SearchCell::_reindex indexing\n";
      m_loader->reset();
      while ( m_loader->next(docs, m_load_batch_size) ) {
        
		for (auto d : docs) {
		  
		  indexer->indexDoc( d.first , d.second );  	
		}
	    docs.clear();
      }
	  
	  std::cout << "SearchCell::_reindex sorting index " << m_index->size() << std::endl;
	  m_index->sort();
	  std::cout << "SearchCell::_reindex dumping index\n";
	  m_index->dump( m_index_fn );
	  
	  // Searcher
	  m_searcher.reset(new Searcher( m_dict, m_index, m_query_compiler ));
	  
	  // Dictionary
	  m_dict->dump();
	}

	void _query(const Query &_usr_query, std::vector<DocId> &_res) {
     
	  m_searcher->query(_usr_query, _res);
	}
};

}

#endif
