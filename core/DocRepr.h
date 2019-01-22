#ifndef _SEARCH_TEXT_REPR_H_
#define _SEARCH_TEXT_REPR_H_

#include "search_defs.h"
#include <vector>
#include <json/json.h>

namespace searchcell {

class DocRepr {
public:
	
  class TokenOccur {
  public:
     TokenOccur(TokenId _token, bool _exact):
	   token(_token),
 	   exact(_exact) {
     }

     TokenOccur(Json::Value &_js) {
       token = _js["t"].asInt();
       exact = _js["e"].asBool();
     }

     Json::Value toJs() const {

     	Json::Value js;
     	js["t"] = token;
     	js["e"] = exact;
     	return js;
     }

     TokenId token;
     bool exact;
  };

  std::vector<TokenOccur> tokens;

  size_t size() const {

    return tokens.size();
  }
  
  DocRepr() { }

 /* DocRepr(const std::string &_dump) {
    
    Json::Value js;
    if (!Json::Reader().parse(_dump, js))
      throw std::invalid_argument("DocRepr could not parse");
    
    for ( auto i = 0; i<js.size(); ++i ) {
      tokens.push_back( TokenOccur( js[i] ) );
    }
  }

  std::string dump() const {
    
    Json::Value js;
    for (auto t : tokens) {
      js.append( t.toJs() );
    }
    return Json::FastWriter().write(js);
  }
  */
};

};

#endif
