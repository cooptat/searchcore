#ifndef STYLESEARCH_SEMANTIC_PARSER_H_
#define STYLESEARCH_SEMANTIC_PARSER_H_

#include "Lexem.h"
#include "SemanticChain.h"
#include "UnigramDetector.h"
#include "NgramDetector.h"

#include "searchcell/strings/Stemmer.h"

namespace style {
namespace lingua {

class SemanticParser {
	
	searchcell::Stemmer m_stemmer;

	std::vector<UnigramDetectorPtr> m_unigram_detectors;
	std::vector<NgramDetectorPtr> m_ngram_detectors;
	
	void detectUnigrams(const std::vector<Token> &_query, SemanticChain &_chain) const;
	void detectNgrams(const std::vector<Token> &_query, SemanticChain &_chain) const;
	
public:
	
  SemanticParser(std::vector<UnigramDetectorPtr> &_detectors);
  
  void parse(const StringUtf8 &_str, SemanticChain &_chain) const;
  void parse(const std::vector<Token> &_query, SemanticChain &_chain) const;
};

typedef boost::shared_ptr<SemanticParser> SemanticParserPtr;

}
}

#endif
