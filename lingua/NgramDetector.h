#ifndef STYLESEARCH_CLASS_NGRAM_DETECTOR_H_
#define STYLESEARCH_CLASS_NGRAM_DETECTOR_H_

#include "Lexem.h"
#include "SemanticChain.h"

namespace style {
namespace lingua {

class NgramDetector {
public:
	
  virtual int detect(const std::vector<Token> &_query, SemanticChain &_chain) const = 0;
  virtual ~NgramDetector() { }
};

typedef boost::shared_ptr<NgramDetector> NgramDetectorPtr;

}
}

#endif
