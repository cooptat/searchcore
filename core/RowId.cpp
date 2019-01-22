#include "RowId.h"
#include <json/json.h>

namespace searchcell {

	RowId::RowId(TokenId _tokenid, bool _exact): tokenid(_tokenid), exact(_exact) { }

	RowId::RowId(const std::string &_dump) {
  
	  Json::Value js;
	  if (!Json::Reader().parse(_dump, js))
	    throw std::invalid_argument("searchcell::RowId::RowId parsing error");
    
	  tokenid = js["t"].asInt();
	  exact = js["e"].asBool();
	}

	std::string RowId::dump() const {
  
	  Json::Value js;
	  js["t"] = tokenid;
	  js["e"] = exact;
	  return Json::FastWriter().write(js);
	}

	bool RowId::operator==(const RowId &_a) const {
		return _a.tokenid == tokenid && _a.exact == exact;
	}

	bool RowId::operator<(const RowId &_a) const {
	
		if (_a.tokenid < tokenid)
			return true;

		if (_a.exact < exact)
			return true;

		return false;
	}

}
