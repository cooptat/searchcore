#ifndef STYLESEARCH_LEXEM_H_
#define STYLESEARCH_LEXEM_H_

#include "LinguaDefs.h"
#include <string>
#include <boost/shared_ptr.hpp>

namespace style {
namespace lingua {

struct Lexem {
public:

	static const int max_distance = 9999999;

	size_t pos;
	bool recognized;
	
	int distance;
	
	Token token;
	Token corrected_token;

	bool corrected;
    bool ask_to_replace;
	
	std::string lex_type;
	std::string lex_class;
	std::string lex_attributes; // age range
	
	Lexem(size_t _pos, const Token &_token);

/*	Lexem(
		size_t _pos,
		int _distance,
		const Token &_token,
	    const Token &_corrected_token,
	    bool _corrected,
	    bool _ask_to_replace,
	    const std::string &_lex_type,
		const std::string &_lex_class,
		const std::string &_lex_attributes
		);
*/

	void print() const;
};

typedef boost::shared_ptr<Lexem> LexemPtr;

}
}

#endif
