#ifndef SRH_ROWID_H_
#define SRH_ROWID_H_

#include "search_defs.h"

namespace searchcell {

class RowId {
public:
		
	TokenId tokenid;
	bool exact;

  RowId() {
  	
  }
	RowId(TokenId _tokenid, bool _exact);
	
	RowId(const std::string &_dump);
	std::string dump() const;

	bool operator==(const RowId &_a) const;
	bool operator<(const RowId &_a) const;
};

}

namespace std {
	template <>
 	struct hash< searchcell::RowId > {

    	std::size_t operator()(const searchcell::RowId &_k) const {
	      	using std::size_t;
	      	using std::hash;
	      	using std::string;

	      	return std::hash<searchcell::TokenId>()(_k.tokenid);
    	}
	};
}

#endif
