#ifndef _LCT_SEARCH_DICT_H_
#define _LCT_SEARCH_DICT_H_

#include "search_defs.h"
#include <unordered_map>
#include <queue>
#include "fs/LevelDb.h"

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <boost/lexical_cast.hpp>

#include <iostream>

/*
	Stores Token->TokenId
*/

namespace searchcell {

//template <
//  template<class T> class Allocator
//>
class Dict {
	
	typedef boost::shared_mutex Lock;
	typedef boost::unique_lock< Lock > WriteLock;
	typedef boost::shared_lock< Lock > ReadLock;
	
	Lock m_lock;

	std::unordered_map<
		Token,
		TokenId,
		std::hash<Token>,
		std::equal_to<Token>
//		, Allocator< std::pair< const Token, TokenId > > 
		> m_tokens;

	LevelDb m_store;
	
	typedef std::queue<
	  std::pair<Token, TokenId>,
	  std::deque <
	  	std::pair<Token, TokenId>
//	    , Allocator< std::pair< Token, TokenId > >
			 > > DumpQueue;
		
		DumpQueue m_dump_queue;

public:

	typedef typename std::shared_ptr<Dict> Ptr;

	Dict(const std::string &_path):
		m_store(_path) {
		
		WriteLock lock(m_lock);
		load();
	}
	
	void clear() {
		
		m_tokens.clear();
		m_dump_queue = DumpQueue();
		m_store.clear();
	}

	void load() {

		auto it = m_store.getIterator();
		it->SeekToFirst();
		while (it->Valid()) {
		
			m_tokens[ it->key().ToString() ] = boost::lexical_cast<TokenId>( it->value().ToString() );
			it->Next();
		}
	}

	void dump() {

		while (!m_dump_queue.empty()) {

			auto t = m_dump_queue.front();
			m_store.put(t.first, std::to_string( t.second ));
			m_dump_queue.pop();
		}
	}

	bool getId(const Token &_t, TokenId &_id) {
		
		ReadLock lock(m_lock);
		auto it = m_tokens.find(_t);
		if (it == m_tokens.end())
			return false;
		
		_id = it->second;
		return true;
	}

	TokenId genId(const Token &_t) {
		
		WriteLock lock(m_lock);
		auto it = m_tokens.find(_t);
		if (it != m_tokens.end())
			return it->second;
		
		TokenId id = m_tokens.size();
		m_tokens.insert(std::make_pair(_t, id));
		m_dump_queue.push( std::make_pair(_t, id) );

		return id;
	}

	size_t size() const {
		
		return m_tokens.size();
	}

	void print() const {
      
      std::cout << "Dict: \n";
      for (auto t : m_tokens) {
      	std::cout << t.first << " | " << t.second << std::endl;
      }
	}
};

typedef typename Dict::Ptr DictPtr;

};

#endif
