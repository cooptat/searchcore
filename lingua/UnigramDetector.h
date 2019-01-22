#ifndef STYLESEARCH_CLASS_UNIGRAM_DETECTOR_H_
#define STYLESEARCH_CLASS_UNIGRAM_DETECTOR_H_

#include "Lexem.h"
#include <boost/shared_ptr.hpp>

#include <map>
#include <vector>

namespace style {
namespace lingua {

class UnigramDetector {

protected:
  std::string m_lex_type;

public:
  
  UnigramDetector(const std::string &_lex_type): m_lex_type(_lex_type) { }

  virtual int detect(const Token &_token, Lexem &_lexem) const = 0;
  virtual ~UnigramDetector() { }

  virtual void dump() = 0;
  virtual void load() = 0;
  virtual void exportPublicLexems( std::map<std::string, std::vector<std::string> > &_public_lex ) const = 0;
};

typedef boost::shared_ptr<UnigramDetector> UnigramDetectorPtr;

}
}

#endif
