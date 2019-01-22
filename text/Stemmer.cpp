#include "Stemmer.h"
//#include "libstemmer.h"

namespace searchcell {

Stemmer::Stemmer() {
	
	m_stemmers[ EN ] = sb_stemmer_new("english", NULL /* UTF_8 */ );
	m_stemmers[ RU ] = sb_stemmer_new("russian", NULL /* UTF_8 */ );
}

Stemmer::~Stemmer() {
	
	for (auto it : m_stemmers) {
		sb_stemmer_delete(it.second);
	}
}

void Stemmer::stem(const std::string &_word, Lang _lang, std::string &_out) {
	
	auto it = m_stemmers.find(_lang);
	if (it == m_stemmers.end()) {
		_out = _word;
		return;
	}
	
	_out = std::string( (const char *) sb_stemmer_stem(it->second, (const sb_symbol *) _word.c_str(), _word.size() ) );
}

};

