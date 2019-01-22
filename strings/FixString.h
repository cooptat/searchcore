#ifndef SEARCHCELL_FIXSTRING_H_
#define SEARCHCELL_FIXSTRING_H_

#include <string>
#include <vector>
#include <iostream>
#include <cstring>

#include "searchcell/contrib/utf8/utf8.h"

namespace searchcell {

template<class LengthT, size_t Capacity>
class FixStringT {

  char m_bf[ Capacity ];
	LengthT m_length;

  void _construct(const std::string &_str);

public:
	
	FixStringT();
	FixStringT(const std::string &_str);
	FixStringT(const FixStringT &_a);
	
	FixStringT& operator=(const FixStringT &_a);
	
	static void getUtf8Prefixes(const std::string &_fix, std::vector<std::string> &_prefixes);
	static std::string truncateUtf8Length(const std::string &_s, size_t _max_length);
	
	std::string toString() const;
	void setLowercase();
	
	bool empty() const {
		
		return m_length == 0;
	}
	
	LengthT length() const {
		
		return m_length;
	}
	
	bool operator==( const FixStringT &_a ) const {
		
		if (m_length != _a.m_length)
			return false;
		
		for (auto i = 0; i < m_length; ++i)
			if (m_bf[i] != _a.m_bf[i])
				return false;
		
		return true;
	}
};

template<class LengthT, size_t Capacity>
FixStringT<LengthT, Capacity>& FixStringT<LengthT, Capacity>::operator=(const FixStringT<LengthT, Capacity> &_a) {
	
	m_length = _a.m_length;
	std::memcpy(m_bf, _a.m_bf, m_length);
	return *this;
}

template<class LengthT, size_t Capacity>
FixStringT<LengthT, Capacity>::FixStringT(const FixStringT<LengthT, Capacity> &_a) {
	
	m_length = _a.m_length;
	std::memcpy(m_bf, _a.m_bf, m_length);
}

template<class LengthT, size_t Capacity>
FixStringT<LengthT, Capacity>::FixStringT():
  m_length(0) {
}

template<class LengthT, size_t Capacity>
void FixStringT<LengthT, Capacity>::_construct(const std::string &_str) {
	
	auto truncated = truncateUtf8Length(_str, Capacity);
	m_length = truncated.size();
	
	std::memcpy(m_bf, truncated.data(), m_length);
}

// constructor
template<class LengthT, size_t Capacity>
FixStringT<LengthT, Capacity>::FixStringT(const std::string &_str) {
	
	_construct(_str);
}

// toString
template<class LengthT, size_t Capacity>
std::string FixStringT<LengthT, Capacity>::toString() const {
	
	return std::string(m_bf, m_length);
}

// getUtf8Prefixes
template<class LengthT, size_t Capacity>
void FixStringT<LengthT, Capacity>::getUtf8Prefixes(const std::string &_fix, std::vector<std::string> &_prefixes) {
	
	char *s = (char*)_fix.data();
	auto fix_size = _fix.size();
	
	std::vector<int> utf32;
	utf8::utf8to32(s, s + fix_size, std::back_inserter(utf32));
	
	for (int i = 1; i<=utf32.size(); ++i) {

		std::vector<unsigned char> utf8;
		utf8::utf32to8( utf32.data(), utf32.data() +  i, std::back_inserter( utf8 ) );
		
		std::string prefix ( (char*)utf8.data(), utf8.size() );
		_prefixes.push_back( prefix );
	}
}

// truncateUtf8Length
template<class LengthT, size_t Capacity>
std::string FixStringT<LengthT, Capacity>::truncateUtf8Length(const std::string &_s, size_t _max_length) {
	
	std::string ret;
	
	std::vector<std::string> prefixes;
	getUtf8Prefixes(_s, prefixes);
	
	for (auto p : prefixes) {
		
		if (p.size() >= _max_length)
			return ret;
		
		ret = p;
	}
	return ret;
}

// setLowercase
template<class LengthT, size_t Capacity>
void FixStringT<LengthT, Capacity>::setLowercase() {
  
	// А-Я 1040-1071
	// а-я 1072-1103
	// A-Z
	// a-z
	
	char *s = (char*)m_bf;
	auto fix_size = m_length;
	
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
	_construct( std::string ( (char*)utf8.data(), utf8.size() ) ) ;	
}

}

#endif
