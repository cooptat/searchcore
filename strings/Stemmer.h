#ifndef SEARCHCELL_STEMMER_H_
#define SEARCHCELL_STEMMER_H_

#include "StringUtf8.h"
#include "searchcell/contrib/libstemmer_c/include/libstemmer.h"

namespace searchcell {

class Stemmer {
	
  struct sb_stemmer * stemmer;
  const char * language = "russian";
  char * charenc = NULL; /* UTF_8 */
	
public:
	
	Stemmer();
	
	StringUtf8 stem(const StringUtf8 &_word) const;
	
};
	
}

#endif
