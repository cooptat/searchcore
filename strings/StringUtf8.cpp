#include "StringUtf8.h"
#include "searchcell/contrib/libstemmer_c/include/libstemmer.h"
#include <iostream>

namespace searchcell {

StringUtf8 StringUtf8::utf32ToString(const std::vector<int> &_utf32) {
  
  std::vector<unsigned char> utf8;
  utf8::utf32to8( _utf32.data(), _utf32.data() + _utf32.size(), std::back_inserter( utf8 ) );
  return StringUtf8 ( (char*)utf8.data(), utf8.size()  );
}

void StringUtf8::fixUtf8() {
		
  std::string temp(this->c_str(), this->length());
	std::string newstr;
  try {
    utf8::replace_invalid(temp.begin(), temp.end(), back_inserter(newstr));
  } catch (...) { }
  this->assign(newstr);
}

std::vector<StringUtf8> StringUtf8::split(int _char) const {
  
  boost::container::flat_set<int> schars;
  schars.insert(_char);

  std::vector<StringUtf8> tokens;
  split(schars, tokens);
  return tokens;
}

void StringUtf8::split(const boost::container::flat_set<int> &_schars, std::vector<StringUtf8> &_tokens) const {
  
  if (this->empty()) {
  	_tokens.push_back( "" );
  	return;
  }

  char *s = (char*)this->data();
  auto s_size = this->size();

  std::vector<int> utf32;
  utf8::utf8to32(s, s + s_size, std::back_inserter(utf32));

  if (utf32.empty())
  	return;
  
  std::vector<int> current_token;
  auto schars_end = _schars.end();

  for (auto i = 0; i<utf32.size(); ++i) {

    if (_schars.find(utf32[i]) != schars_end) {

      if (current_token.empty()) {
      	//++i;
      	_tokens.push_back( "" );
      	continue;
      }
      
      auto utf8str = utf32ToString( current_token );
      
      _tokens.push_back( utf8str );
      current_token.clear();
      //++i;
      continue;
    }

    current_token.push_back( utf32[i] );
  }

  if (!current_token.empty()) {
    
    auto utf8str = utf32ToString( current_token );
    _tokens.push_back( utf8str );
  } else
    _tokens.push_back( "" );
}

void StringUtf8::toLower() {
  
	// А-Я 1040-1071
	// а-я 1072-1103
	// A-Z
	// a-z

	char *s = (char*)this->data();
	auto fix_size = this->size();

	std::vector<int> utf32;
	utf8::utf8to32(s, s + fix_size, std::back_inserter(utf32));

	for (auto i = 0; i < utf32.size(); ++i) {
		
		auto v = utf32[i];
		if ( v >= 1040 && v <= 1071 ) {
			utf32[i] = v + 1072-1040;
			continue;
		}
		if (v >= 'A' && v <= 'Z') {
			
			utf32[i] = v + 'a' - 'A';
			continue;
		}
	}

	std::vector<unsigned char> utf8;
	utf8::utf32to8( utf32.data(), utf32.data() +  utf32.size(), std::back_inserter( utf8 ) );
	this->assign( (char*)utf8.data(), utf8.size() );
}

void StringUtf8::getPrefixes(std::vector<StringUtf8> &_prefixes) {
		
	char *s = (char*)this->data();
	auto fix_size = this->size();
		
		std::vector<int> utf32;
		utf8::utf8to32(s, s + fix_size, std::back_inserter(utf32));
		
		for (auto i = 1; i<utf32.size(); ++i) {
	
			std::vector<unsigned char> utf8;
			utf8::utf32to8( utf32.data(), utf32.data() +  i, std::back_inserter( utf8 ) );
			
			StringUtf8 prefix ( (char*)utf8.data(), utf8.size() );
			_prefixes.push_back( prefix );
		}
	}
}
