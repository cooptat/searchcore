#ifndef _SEARCH_TOKENIZER_H_
#define _SEARCH_TOKENIZER_H_

#include "core/search_defs.h"
#include "core/DocRepr.h"
#include "core/Dict.h"
#include "core/QueryRepr.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string.hpp>

#include "StringUtils.h"

#include "LangDetect.h"
#include "Stemmer.h"

namespace searchcell {

template <
  class Dict
>
class Tokenizer {
public:
	
	typedef typename Dict::Ptr DictPtr;

	void splitByDelims_(const std::string &_text, std::vector<std::string> &_words) const  {
	
		boost::split(_words, _text, boost::is_any_of("., ;:	-?&(){}\"\'<>\n")); // do not add "!"
	}
	
	Tokenizer(DictPtr _dict, const std::string &_text, QueryRepr &_query) {
		
		_query.tokens.clear();
		Stemmer stemmer;
		
		std::string text (_text);
		
		toLowerUtf8(text);
		fix_utf8_string(text);
		
		std::vector<std::string> words;
		splitByDelims_(text, words);
		
		for (int i = 0; i<words.size(); ++i)
			if (words[i].size() != 0) {
				
				std::string word = words[i];
				
				bool need_exact = false;
				if (word[0] == '!') {
					need_exact = true;
					word = word.substr(1);
				}
				
				std::string stem;				
				stemmer.stem(words[i], LangDetector::detect(words[i]), stem);
				
				TokenId word_id, stem_id;
				
				if (_dict->getId(word, word_id) && _dict->getId(stem, stem_id))
					_query.tokens.push_back( QueryToken(need_exact, word_id, stem_id) );			
			}
	}
	
	Tokenizer(DictPtr _dict, const std::string &_text, DocRepr &_textrepr) {
		
		Stemmer stemmer;
		
		std::string text (_text);
		
		toLowerUtf8(text);
		fix_utf8_string(text);
		
		std::vector<std::string> words;
		splitByDelims_(text, words);
	
		for (int i = 0; i<words.size(); ++i)
			if (words[i].size() != 0) {
					
				std::string word = words[i];
				auto lang = LangDetector::detect(word);

				std::string stem;				
				stemmer.stem(word, lang, stem);
				
				auto tid = _dict->genId(word);
				
				_textrepr.tokens.push_back( DocRepr::TokenOccur(tid, true) );
				if (stem != word) {
					
					auto tid = _dict->genId(stem);
					_textrepr.tokens.push_back( DocRepr::TokenOccur(tid, false) );
				}
			}
	}
};

};

#endif

