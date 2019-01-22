#include <cxxtest/TestSuite.h>
#include "searchcell/strings/Speller.h"

#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

class TestSpellerClass : public CxxTest::TestSuite {
public:

	void print_matches(std::ostream& out, const RadixTrie::matches_t& matches)
	{
	    out << "[";
	    for (unsigned i = 0; i < matches.size(); i++)
	    {
	        const auto& r = matches[i];
	        out << "{" <<
	            "\"word\":\"" << r.word << "\","
	            "\"freq\":" << r.freq << ","
	            "\"distance\":" << r.distance << "}";
	        if (i != matches.size() - 1)
	            out << ",";
	    }
	    out << "]" << std::endl;
	}


void testSpeller() {
  
	std::string words_fn = "words.txt";
	std::string dict_fn = "dict.bin";
	
	{
	  searchcell::Speller speller;
		
		speller.addWord(1, "блондинки");
		speller.addWord(1, "брюнетки");
		
		speller.dump(dict_fn);
		
		//auto res = speller.match("блоднинка", 5);
	  //print_matches(std::cout, res);		
	}
	{
	  searchcell::Speller speller;
		speller.load(dict_fn);
		
		auto res = speller.match("блоднинка", 5);
	  print_matches(std::cout, res);
	}
	
}

};
