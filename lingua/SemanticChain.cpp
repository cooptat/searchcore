#include "SemanticChain.h"
#include <iostream>

namespace style {
namespace lingua {

SemanticChain::SemanticChain() {
}

SemanticChain::SemanticChain(const std::vector<Token> &_tokens) {
    
    for (auto i = 0; i < _tokens.size(); ++i) {
	    
	positions.push_back( Position( i, _tokens[i] ) );
    }
}

bool SemanticChain::Position::containsClass(const std::string &_type, const std::string &_class) const {
  
  for (auto l : lexems) {

  	if (l.recognized && _type == l.lex_type && _class == l.lex_class)
  	  return true; 
  }
  return false;
}

void SemanticChain::addLexem(const Lexem &_lex) {

  while (positions.size() <= _lex.pos) {
    positions.push_back( Position( positions.size(), _lex.token ) );
  }
  positions[ _lex.pos ].lexems.push_back(_lex);
}

bool SemanticChain::containsClass(size_t _pos, const std::string &_type, const std::string &_class) const {

  if (_pos >= positions.size())
  	return false;

  if (positions[_pos].containsClass(_type, _class))
  	return true;

  return false;
}

void SemanticChain::print() {
  
  std::cout << "SemanticChain size: " << positions.size() << std::endl;

  for (auto p : positions) {

  	std::cout << "[" << p.pos << "]\n";

  	for (auto l : p.lexems) {

  		l.print();
  	}

  	std::cout << std::endl;
  }
}

}
}
