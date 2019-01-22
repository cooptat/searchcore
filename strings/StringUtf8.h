#ifndef STRING_UTF8_H_
#define STRING_UTF8_H_

#include <string>
#include <boost/container/flat_set.hpp>
#include <vector>
#include "searchcell/contrib/utf8/utf8.h"

namespace searchcell {

class StringUtf8 : public std::string {
public:

  using std::string::string;

  StringUtf8( const std::string& _a ):
    std::string(_a) {
  }

  static StringUtf8 utf32ToString(const std::vector<int> &_utf32);

  void fixUtf8();
  void split(const boost::container::flat_set<int> &_schars, std::vector<StringUtf8> &_tokens) const;
  void toLower();
  void getPrefixes(std::vector<StringUtf8> &_prefixes);

  std::vector<StringUtf8> split(int _char) const;
};

}

namespace std {
    template <>
    struct hash<searchcell::StringUtf8> {
        std::size_t operator()(const searchcell::StringUtf8 &s) const {
            return std::hash<std::string>()(s);            }
    };
}

#endif
