#ifndef CLOSED_MAP_H_
#define CLOSED_MAP_H_

#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace searchcell {

template<class K, class V, class Hash>
class ClosedMapImplT {

  const std::vector<int> m_primes = {
  	1009, 10007, 50021, 100003, 500009, 1000003, 10000019, 20000003, 30000001, 40000003, 50000017,
		60000011, 70000027, 80000023, 90000049, 100000007, 110000017, 120000007, 130000001, 140000041,
		150000001, 160000003, 170000009, 180000017, 190000003, 200000033, 210000017, 220000013, 
		236887699, 256203221, 275604547, 295075153, 314606891, 334214467, 353868019,
		373587911, 393342743, 413158523, 433024253, 452930477, 472882049, 492876863,
		512927377, 533000401, 553105253, 573259433, 593441861, 613651369, 633910111,
		654188429, 674506111, 694847539, 715225741, 735632797, 756065179, 776531419,
		797003437, 817504253, 838041647, 858599509, 879190841, 899809363, 920419823,
		941083987, 982451653
  } ;
	
public:
  int usedslots = 0;

  std::vector<K> keys;
  std::vector<V> vals;
  std::vector<uint8_t> used;

  //size0 should be a prime and about 30% larger than the maximum number needed
  ClosedMapImplT(int _size) {
		
		int size0 = -1;
		for (auto p : m_primes) {
			
			if ( int(_size * 1.3) < p) {
				size0 = p;
			  break;
			}
		}
		if (size0 == -1)
			throw std::runtime_error("searchcell::ClosedMap need more prime numbers");
		
    vals.resize(size0);
    keys.resize(size0);
    used.resize(size0/8+1,0);
  }
	
	ClosedMapImplT(const std::string &_fn) {
		
		restore(_fn);
	}
	
  //If the key values are already uniformly distributed, using a hash gains us
  //nothing
  uint64_t hash(const K &key) const {
    
	//	std::cout << "hash: " << key << std::endl;
		return Hash()(key);
  }

  bool isUsed(const uint64_t loc) const {
    
		const auto used_loc = loc/8;
    const auto used_bit = 1<<(loc%8);
    return used[used_loc] & used_bit;
  }

  void setUsed(const uint64_t loc) {
    
		const auto used_loc = loc/8;
    const auto used_bit = 1<<(loc%8);
    used[used_loc] |= used_bit;
  }

  void insert(const K &key, const V &val) {
    
		if (size() > 0.8  * capacity()) {
			throw std::invalid_argument("searchcell::ClosedMap filled >80% capacity");
		}
		
		uint64_t loc = hash(key) % keys.size();

    //Use linear probing. Can create infinite loops if table too full.
    while (isUsed(loc)) {
			
			loc = (loc+1) % keys.size();
		}

    setUsed(loc);
    keys[loc] = key;
    vals[loc] = val;
		++usedslots;
  }

  V& get(const K& key) {
		
    uint64_t loc = hash(key) % keys.size();

    while(true) {
      if ( !isUsed(loc) ) {
        throw std::runtime_error("ClosedMap::get Item not present!");
      //	continue;
      }
      if (keys[loc] == key)
        return vals[loc];

      loc = (loc+1) % keys.size();
    }
  }

  uint64_t size() const {
    
		return usedslots;
  }

  uint64_t capacity() const {
    
		return keys.size();
  }
	
	float getFillFactor() const {
		return float(size()) / float(capacity());
	}
	
	void dump(const std::string &_fn) {
		
		std::ofstream o( _fn.c_str(), std::ios::out | std::ios::binary );
		
		int cap = capacity();

	//	std::cout << "Dump cap: " << cap << std::endl;

		o.write( (char*) &cap, sizeof(int) / sizeof(char) );
		o.write( (char*) &usedslots, sizeof(int) / sizeof(char) );
		o.write( (char*) keys.data(), cap * sizeof(K) / sizeof(char) );
		o.write( (char*) vals.data(), cap * sizeof(V) / sizeof(char) );
		o.write( (char*) used.data(), sizeof(uint8_t) * (cap / 8 + 1 ) / sizeof(char) );
	}
	
	void restore(const std::string &_fn) {
		
		std::ifstream f( _fn.c_str(), std::ios::binary );
		
	        int cap;
		f.read( (char*) &cap, sizeof(int) / sizeof(char) );	
		f.read( (char*) &usedslots, sizeof(int) / sizeof(char) );	
		
	//	std::cout << "Restore cap: " << cap << std::endl;
	//	std::cout << "assign vectors keys" << std::endl;

		keys.resize(cap);
		
	//	std::cout << "assign vectors vals" << std::endl;
		vals.resize(cap);
		
	//	std::cout << "assign vectors used" << std::endl;
		used.resize(cap/8+1);
		
	//	std::cout << "Restore keys " << std::endl;
		
		f.read( (char*) keys.data(), cap * sizeof(K) / sizeof(char) );
		
//		std::cout << "Restore vals " << std::endl;
		
		f.read( (char*) vals.data(), cap * sizeof(V) / sizeof(char));
		
	//	std::cout << "Restore used " << std::endl;
		
		f.read( (char*) used.data(), sizeof(uint8_t) * (cap / 8 + 1) / sizeof(char) );
	}
};

template<class K, class V, class Hash>
class ClosedMap {
	
	typedef ClosedMapImplT<K,V,Hash> ClosedMapImpl;
	typedef boost::shared_ptr< ClosedMapImpl > ClosedMapImplPtr;
	
	ClosedMapImplPtr m_impl;
	ClosedMapImplPtr m_impl_enlarged;
	
	const int m_default_capacity = 1024;
	const float m_enlarge_threshold = 0.7;
	const float m_enlarge_factor = 1.4;
	
public:
	
	ClosedMap() {
		
		m_impl.reset(new ClosedMapImpl( m_default_capacity ));
	}
	
	ClosedMap(int _capacity) {
		
	  m_impl.reset(new ClosedMapImpl(_capacity));	
	}
	
	ClosedMap(const std::string &_fn) {
		
		m_impl.reset(new ClosedMapImpl(_fn));
	}
	
	void enlarge() {
		
//		std::cout << "ClosedMap enlarging from " << capacity() << " to " << capacity() * m_enlarge_factor  << "..";
		
		m_impl_enlarged.reset(new ClosedMapImpl( capacity() * m_enlarge_factor ));
				
		for (auto i = 0; i < m_impl->keys.size(); ++i) {
			
			if (!m_impl->isUsed(i))
				continue;

			K &k = m_impl->keys[i];
			try {
			  V &v = m_impl->get( k );
				m_impl_enlarged->insert( k, v );
				
			} catch (...) {
			}
		}
		
		m_impl = m_impl_enlarged;
	}

	void print() {


		std::cout << "MAp print\n";

		for (auto i = 0; i < m_impl->keys.size(); ++i) {
			
			//if (!m_impl->isUsed(i))
			//	continue;

			K &k = m_impl->keys[i];
			//std::cout << k << " ";

			try {
 


			  V &v = m_impl->get( k );
			  
		      std::cout << v << ":";
			  

			} catch (...) {
			}
		}
	}
	
  uint64_t size() const {
    
		return m_impl->size();
  }

  uint64_t capacity() const {
    
		return m_impl->capacity();
  }
	
	void insert(const K& key, const V& val) {
	  
		if (m_impl->getFillFactor() > m_enlarge_threshold) {
			
			enlarge();
		}
		
		m_impl->insert(key, val);
	}
	
	V& get(const K& key) {
	  
		return m_impl->get(key);
	}
	
	void dump(const std::string &_fn) {
	  
		m_impl->dump(_fn);	
	}
	
	void restore(const std::string &_fn) {
		
		m_impl->restore(_fn);
	}
	
	void clear() {
		
		m_impl.reset(new ClosedMapImpl( m_default_capacity ));
		m_impl_enlarged.reset();
	}
};

}

#endif
