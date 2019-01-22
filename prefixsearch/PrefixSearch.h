#ifndef PREFIX_SEARCH_H_
#define PREFIX_SEARCH_H_

#include <unordered_map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>

#include "searchcell/core/ClosedMap.h"

#include "searchcell/contrib/utf8/utf8.h"

#include "searchcell/strings/FixString.h"

namespace prefix {

template<size_t FixSize, size_t TopSize, size_t MAX_PREFIX_LEN>
class Search {
public:
    
  static constexpr size_t max_prefix_len = MAX_PREFIX_LEN;
    
  typedef boost::shared_ptr< Search > Ptr;
  
  struct FixStats {
    int fix_id;
    int freq;
  };

  typedef searchcell::FixStringT<uint8_t, FixSize> Fix;
	
  struct FixHash {
    
		FixHash() {}
		
    size_t operator()(const Fix &_fix) const {

      return std::hash<std::string>()( _fix.toString() );
    }
  };

	struct Top {
	  
		int top[ TopSize ];
		
		Top(const std::vector<int> &_v) {
			
			auto i = 0;
			while (i<_v.size()) {
				
				if (i  >= TopSize) {
					top[i] = -1;
					return;
				}
				
				top[i] = _v[i];
				++i;
			}
			if (i < TopSize)
				top[i] = -1;	
		}
		
		Top() {
			
				top[0] = -1;
		}
		Top(const Top &_a) {
			
			for (auto i = 0; i<TopSize; ++i)
				top[i] = _a.top[i];
		}
		Top& operator=(const Top &_a) {
			
			for (auto i = 0; i<TopSize; ++i)
				top[i] = _a.top[i];
			
			return *this;
		}
		
		int size() const {
			for (auto i = 0; i < TopSize; ++i) {
				if (top[i] == -1)
					return i;
			}
			throw std::runtime_error("PrefixSearxh::Top must be terminated with -1");
		}
	};
	
  std::unordered_map<Fix, FixStats, FixHash> m_fix_stats; // temporary
  std::unordered_map<Fix, std::vector< FixStats >, FixHash > m_prefixes_intermediate; // temporary

  searchcell::ClosedMap<int, Fix, std::hash<int> > m_id_fixes;
  searchcell::ClosedMap<Fix, Top, FixHash> m_prefixes_top;

  bool m_sorted = false;

  std::string m_dump_ids_fn;
  std::string m_dump_top_fn;
	
public:

	void printInter() {
		
	/*	std::cout << "fix stats: \n";
		for (auto it : m_fix_stats) {
			
			std::cout << it.first.str << " (" << it.second.fix_id << ", " << it.second.freq << ")\n";
		}
		
			std::cout << "prefixes inter: \n";
		
		for (auto it : m_prefixes_intermediate) {
			
			std::cout << it.first.str << ": ";
			for (auto fs : it.second) {
			  std::cout << "(" << fs.fix_id << ", " << fs.freq << ") ";
		  }
			std::cout << std::endl;
		} */
	}

  Search(const std::string &_dump_fn):
    m_dump_ids_fn(_dump_fn + "_ids"),
    m_dump_top_fn(_dump_fn + "_top") { }

  size_t size() const {
  	return m_id_fixes.size();
  }

  void clear() {

  	m_fix_stats.clear();
  	m_prefixes_intermediate.clear();
  	m_id_fixes.clear();
  	m_prefixes_top.clear();
  }
	/*
	void fix_utf8_string(std::string& str) {
		
	    std::string temp;
			try {
	      utf8::replace_invalid(str.begin(), str.end(), back_inserter(temp));
		  } catch (...) { }
	    str = temp;
	}*/

  void addFix(const std::string &_fix) {

		if (_fix.size() <= 1)
			return;

    Fix fix(_fix);

  	auto it = m_fix_stats.find( fix );
  	if (it == m_fix_stats.end()) {

  	  int id = m_id_fixes.size();
  	  m_id_fixes.insert(id, fix);
			
  	  m_fix_stats[ fix ] = FixStats{ id, 1 };
  	} else {

  	  ++it->second.freq;
  	}
  }

  void sort() {

  	for (auto it : m_fix_stats) {

  	  const Fix& fix = it.first;
			
			std::vector<std::string> prefixes;
			Fix::getUtf8Prefixes(fix.toString(), prefixes);
			
			auto i = 0;
			for (auto prefix_str : prefixes)	{
				
				if (i >= MAX_PREFIX_LEN)
					break;

				++i;

				auto prefix = Fix( prefix_str );
				prefix.setLowercase();
				m_prefixes_intermediate[ prefix ].push_back( it.second );
			}
  	}

  	for (auto it : m_prefixes_intermediate) {

  	  const Fix& prefix = it.first;

  	  std::sort(it.second.begin(), it.second.end(), [](const FixStats &_a, const FixStats &_b) {

        return _a.freq > _b.freq;
  	  });


  	  std::vector<int> top;
  	  for (auto sit : it.second) {
  	  	
  	  	if (top.size() >= TopSize)
  	  	  break;
				
  	  	top.push_back( sit.fix_id );
  	  }

  	  m_prefixes_top.insert( prefix , Top(top) );
  	}
    		
    m_fix_stats.clear();
  	m_prefixes_intermediate.clear();
  	m_sorted = true;
  }

  void query(const std::string &_prefix, std::vector<std::string> &_fixes) {
    
    if (!m_sorted)
      throw std::invalid_argument("prefix::Search::query not sorted");
				
		try {
			
			Fix prefix(_prefix);
			prefix.setLowercase();
			
			Top& top = m_prefixes_top.get( prefix );
						
			for (auto i = 0; i<TopSize; ++i) {
									
				if (top.top[i] == -1)
					break;
				
				auto fix_id = top.top[i];				
				_fixes.push_back( m_id_fixes.get(fix_id).toString() );
			}
					
	  } catch (...) {
	  	
	  }
  }

  void dump() {
    
    if (!m_sorted)
      throw std::invalid_argument("prefix::Search::dump not sorted");

    m_id_fixes.dump(m_dump_ids_fn);
		m_prefixes_top.dump(m_dump_top_fn);
  }

  void load() {
    
	  m_id_fixes.restore(m_dump_ids_fn);
		m_prefixes_top.restore(m_dump_top_fn);
		
		m_sorted = true;
  }
};

}

#endif
