#ifndef SEARCHCELL_SPELLER_H_
#define SEARCHCELL_SPELLER_H_

#include "searchcell/contrib/ouiche/src/radix-trie.hh"
#include "searchcell/contrib/ouiche/src/compact-radix-trie.hh"

namespace searchcell {

class Speller {

  std::unique_ptr<RadixTrie> m_trie;

  int fd;
  void* file;
  char* start;

  bool m_ready;

public:

  Speller();

  void dump(const std::string &_dump_fn);
  void load(const std::string &_dump_fn);

  void addWord(unsigned freq, const std::string& word);

	CompactRadixTrie::matches_t match(const std::string &_word, unsigned _max_distance) const;
}; 

}

#endif

