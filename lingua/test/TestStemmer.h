#include <cxxtest/TestSuite.h>
#include "searchcell/contrib/libstemmer_c/include/libstemmer.h"
#include <exception>
#include <iostream>

class TestStemmer : public CxxTest::TestSuite {
public:

void testStemmer() {
   
  try {
    
    struct sb_stemmer * stemmer;
    const char * language = "russian";
    char * charenc = NULL; /* UTF_8 */

    stemmer = sb_stemmer_new(language, charenc);
    if (stemmer == 0) {
        if (charenc == NULL) {
            fprintf(stderr, "language `%s' not available for stemming\n", language);
            exit(1);
        } else {
            fprintf(stderr, "language `%s' not available for stemming in encoding `%s'\n", language, charenc);
            exit(1);
        }
   }

   std::string word = "лингвистический";

   std::string stem ( (const char*) sb_stemmer_stem(stemmer, (const sb_symbol *) word.data(), word.size() ));

   std::string stem_test = "лингвистическ";

   TS_ASSERT(stem == stem_test);
   
   sb_stemmer_delete(stemmer);

	  
    } catch (std::exception &_ex) {
  	
	  std::cout << "TestStemmer " <<  _ex.what() << std::endl;
    }
 }

};
