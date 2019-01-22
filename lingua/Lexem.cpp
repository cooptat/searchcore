#include "Lexem.h"
#include <iostream>

namespace style {
namespace lingua {

const int Lexem::max_distance;

Lexem::Lexem(size_t _pos, const Token &_token):
	 pos(_pos),
	 recognized(false),
	 token(_token) {
	}
/*
Lexem::Lexem(
		size_t _pos,
		int _distance,
		const Token &_token,
	    const Token &_corrected_token,
	    bool _corrected,
	    bool _ask_to_replace,
	    const std::string &_lex_type,
		const std::string &_lex_class,
		const std::string &_lex_attributes
		):
pos(_pos),
distance(_distance),
token(_token),
corrected_token(_corrected_token),
corrected(_corrected),
ask_to_replace(_ask_to_replace),
lex_type(_lex_type),
lex_class(_lex_class),
lex_attributes(_lex_attributes)
{ }
*/
void Lexem::print() const {
  
  std::cout
    << "token exact: " << token.exact << std::endl
  	<< "token stem: " << token.exact << std::endl
  	<< "corrected_token: " << corrected_token.exact << std::endl
  	<< "ask_to_replace: " << ask_to_replace << std::endl
  	<< "type: " << lex_type << std::endl
  	<< "class: " << lex_class << std::endl;
}

}
}
