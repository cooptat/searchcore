#ifndef _SEARCH_STEMMER_H_
#define _SEARCH_STEMMER_H_

#include "LangDetect.h"
#include "libstemmer.h"

#include <map>

namespace searchcell {

class Stemmer {
	
	
	std::map<Lang, sb_stemmer*> m_stemmers; 
	
public:
	
	Stemmer();
	~Stemmer();
	void stem(const std::string &_word, Lang _lang, std::string &_out);
};
	
};

#endif
