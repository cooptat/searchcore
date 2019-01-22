#ifndef _SEARCHER_QUERY_REPR_H_
#define _SEARCHER_QUERY_REPR_H_

#include "search_defs.h"
#include <iostream>

namespace searchcell {

class QueryRepr {
public:
	
	class QueryToken {
	public:
		bool need_exact;
		TokenId word;
		TokenId stem;
		int npostings;
		int nfiltered;
	
		QueryToken(bool _need_exact, TokenId _word, TokenId _stem):
			need_exact(_need_exact),
			word(_word),
			stem(_stem),
			npostings(-1),
			nfiltered(-1) {
		}	
	};
	
	std::vector<QueryToken> tokens;
	
	size_t size() const {
		
		return tokens.size();
	}
    void print() const {

      std::cout << "Query:\n";
      for (auto t : tokens) {
    	std::cout << t.stem << " " << std::endl;
      }
    }		
};
	
};

#endif
