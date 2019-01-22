#include "SemanticParser.h"
#include <iostream>

namespace style {
namespace lingua {

SemanticParser::SemanticParser(std::vector<UnigramDetectorPtr> &_detectors):
  m_unigram_detectors(_detectors) {
}

void SemanticParser::parse(const StringUtf8 &_str, SemanticChain &_chain) const {

  std::vector<Token> query;  

  boost::container::flat_set<int> schar;
  schar.insert(32); // utf32 space
  //schar.insert(9); // tab

  //std::cout << "sem query " << _str << std::endl;

  std::vector<StringUtf8> exact_tokens;
  _str.split(schar, exact_tokens);

  for (auto exact : exact_tokens) {

  	if (exact.empty())
  	  continue;

  	auto stem = m_stemmer.stem(exact);

  	if (stem.empty()) {

  	  stem = exact;
  	}

  	query.push_back({ exact, stem });
  }

  parse(query, _chain);
}

void SemanticParser::parse(const std::vector<Token> &_query, SemanticChain &_chain) const {
	
	detectUnigrams(_query, _chain);
	detectNgrams(_query, _chain);
}

void SemanticParser::detectUnigrams(const std::vector<Token> &_query, SemanticChain &_chain) const {

	/*
    	на одной позиции может быть 2 и более лексемы, если у обеих расстояние 0
	  иначе 1 лексема с минимальным расстоянием
	
	  Блондинка - цвет волос и пол
	*/
	
	_chain = SemanticChain(_query);
	
	int pos = 0;
	for (auto token : _query) {
				
		std::vector<Lexem> exact_matches;
		
		Lexem best_lexem(pos, token);
		
		int min_distance = Lexem::max_distance;
		for (auto detector : m_unigram_detectors) {
			
			int distance = detector->detect( token, best_lexem  );

			if (!best_lexem.recognized)
				continue;

			if (distance < min_distance)
				min_distance = distance;
			
			if (distance == 0) {	
				exact_matches.push_back( best_lexem );
			}
		}
		
		if (!exact_matches.empty()) {
		  
			for (auto exact_lexem : exact_matches) {
			  
			  	//std::cout << "pushing exact " << pos << " " << best_lexem.token.exact << std::endl;
				_chain.addLexem( exact_lexem );
			}	
			++pos;
			continue;
		} 
		
		if (best_lexem.recognized) {

		  //std::cout << "pushing best lexem " << pos << " " << best_lexem.token.exact << " distance " << best_lexem.distance << std::endl;
		  _chain.addLexem( best_lexem );
		  ++pos;
		  continue;
		}

		++pos;
	}
}

void SemanticParser::detectNgrams(const std::vector<Token> &_query, SemanticChain &_chain) const {
	
}

}
}
