#include "Speller.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <fstream>

namespace searchcell {

Speller::Speller():
  m_trie(std::make_unique<RadixTrie>()),
  m_ready(false) {
}

void Speller::dump(const std::string &_dump_fn) {

  std::ofstream dict_f(_dump_fn);	
  m_trie->serialize_compact(dict_f);

  load(_dump_fn);
}

void Speller::load(const std::string &_dump_fn) {

  m_trie.reset();

  fd = -1;
 
  if ((fd = open(_dump_fn.c_str(), 0)) == -1)
    abort();

  struct stat s;
  if (fstat(fd, &s) < 0)
    abort();

  file = mmap(NULL, s.st_size, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
  start = reinterpret_cast<char*>(file);

  m_ready = true;
}

void Speller::addWord(unsigned freq, const std::string& word) {

  m_trie->add_word(freq, word);
}

CompactRadixTrie::matches_t Speller::match(const std::string &_word, unsigned _max_distance) const {
	
	return CompactRadixTrie::matches(_word, start, _max_distance);
}

}
