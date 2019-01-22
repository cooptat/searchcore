#ifndef STYLESEARCH_SEMANTIC_CHAIN_H_
#define STYLESEARCH_SEMANTIC_CHAIN_H_

#include "Lexem.h"
#include <vector>
#include <string>

namespace style {
namespace lingua {

struct SemanticChain {

	    struct Position {
			size_t pos;
			std::vector<Lexem> lexems;
			
			Token token;
			
			Position(size_t _pos, const Token &_t):
			  pos(_pos),
			  token(_t) {
			}

			bool containsClass(const std::string &_type, const std::string &_class) const;
		};
	
		std::vector<Position> positions;
		
		SemanticChain();
		SemanticChain(const std::vector<Token> &_tokens);
		
		void addLexem(const Lexem &_lex);
		bool containsClass(size_t _pos, const std::string &_type, const std::string &_class) const;

		void print();
};
}
}

#endif
