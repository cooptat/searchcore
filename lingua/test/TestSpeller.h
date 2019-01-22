#include <cxxtest/TestSuite.h>
#include "searchcell/contrib/ouiche/src/radix-trie.hh"
#include "searchcell/contrib/ouiche/src/compact-radix-trie.hh"

#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

class TestSpeller : public CxxTest::TestSuite {
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
	std::ifstream words_f(words_fn);
  auto trie = std::make_unique<RadixTrie>();
  trie->load(words_f);
  words_f.close();
	
	std::ofstream dict_f(dict_fn);
	
	trie->serialize_compact(dict_f);
	dict_f.close();
  }
	{
		int fd = -1;
    if ((fd = open(dict_fn.c_str(), 0)) == -1)
        abort();

    struct stat s;
    if (fstat(fd, &s) < 0)
        abort();

    void* file = mmap(NULL, s.st_size, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
    char* start = reinterpret_cast<char*>(file);
	
		int max_dist = 6;
		std::string word = "блоднинка";
	
		auto res = CompactRadixTrie::matches(word, start, max_dist);
	  print_matches(std::cout, res);		
	}
	
}

};
