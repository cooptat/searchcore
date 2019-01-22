#include "TestWordDetJson.h"

using namespace style::lingua;
/*
void TestUnigramDetectorJson::testExact() {
   
  try {
    
    HairUniDet det("dump.bin");

    Token token;
    token.exact = "блондинки";
    token.stem = "блондин_";
    Lexem lex(0);

    TS_ASSERT( det.detect(token, lex) == 0);
    TS_ASSERT( lex.corrected == false );
    TS_ASSERT( lex.distance == 0 );

    TS_ASSERT( lex.lex_type == "HAIR" );
    TS_ASSERT( lex.lex_class == "BLONDE" );
    
	  
  } catch (std::exception &_ex) {
  	
	  std::cout << "TestUnigramDetectorJson " <<  _ex.what() << std::endl;
  }
}

void TestUnigramDetectorJson::testStem() {
   
  try {
    
    HairUniDet det("dump.bin");

    Token token;
    token.exact = "блондинки_";
    token.stem = "блондинк";
    Lexem lex(0);

		auto dist = det.detect(token, lex);

    TS_ASSERT( dist > 0);
    TS_ASSERT( lex.corrected == false );
	  TS_ASSERT( lex.distance > 0 );

    TS_ASSERT( lex.lex_type == "HAIR" );
    TS_ASSERT( lex.lex_class == "BLONDE" );
    
	  
  } catch (std::exception &_ex) {
  	
	  std::cout << "TestUnigramDetectorJson " <<  _ex.what() << std::endl;
  }
}*/

void TestWordDetJson::testSpell() {
	
  try {
    
		/*
		const std::string &_detector_type,
		const std::vector<std::string> &_lexem_meta_tuned_fn,
		const std::string &_speller_dump_fn,
		unsigned _default_spell_distance_limit = 10,
		unsigned _default_spell_distance_ask_correct = 4
		*/
		
    WordDetJson det(
			"SEX",
			{
				"lex/sex.lex"
		  },
			"speller.bin",
		  10,
			4,
      0.50f,
      0.49f
		);

		det.dump();

    Token token;
    token.exact = "деувшки";
    token.stem = "";
    Lexem lex(0, token);
		
		auto dist = det.detect(token, lex);

    TS_ASSERT( dist > 0);
    TS_ASSERT( lex.corrected == true );
	  TS_ASSERT( lex.distance > 0 );

    TS_ASSERT( lex.lex_type == "SEX" );
    TS_ASSERT( lex.lex_class == "F" );
		
		std::cout << "Corrected " << lex.token.exact
			<< " " << lex.distance
			<< " ask to replace " << lex.ask_to_replace << std::endl;
    
	  
  } catch (std::exception &_ex) {
  	
	  std::cout << "TestWordDetJson " <<  _ex.what() << std::endl;
  }
}
