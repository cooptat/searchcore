#ifndef _SEARCH_LANG_DETECT_H_
#define _SEARCH_LANG_DETECT_H_

#include <string>
#include <iostream>

namespace searchcell {

enum Lang {
	EN,
	RU,
	UNKNOWN_LANG
};

std::ostream& operator<<(std::ostream& os, const Lang& lang);

class LangDetector {
public:
	
	static Lang detect(const std::string &_str);
};
	
};

#endif
