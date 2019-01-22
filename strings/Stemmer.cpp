#include "Stemmer.h"

namespace searchcell {

Stemmer::Stemmer() {
  
	stemmer = sb_stemmer_new(language, charenc);
  if (stemmer == 0) {
      if (charenc == NULL) {
					throw std::runtime_error("searchcell::Stemmer::Stemmer no language " + std::string(language));
      } else {
				  throw std::runtime_error("searchcell::Stemmer::Stemmer no language for encoding " + std::string(language));
      }
 }
}

StringUtf8 Stemmer::stem(const StringUtf8 &_word) const {
	
	return StringUtf8 ( (const char*) sb_stemmer_stem(stemmer, (const sb_symbol *) _word.data(), _word.size() ));
}	

}
