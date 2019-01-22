#include "SemanticSample.h"

#include <iostream>

namespace style {
namespace lingua {

bool SemanticSample::testFalseNegativeOk(const SemanticChain &_chain) const {

  for (int p = 0; p<expected_semantics.size(); ++p) {

    auto &expected = expected_semantics[p];

    if (!expected.empty) {
      if (!_chain.containsClass(p, expected.lex_type, expected.lex_class)) {

        std::cout << "Test false negative. Must contain class " << expected.lex_class << "(" << expected.lex_type << ") pos: " << p << std::endl;
        return false;
      }
    }
  }
  return true;
}

bool SemanticSample::containsClass(size_t _pos, const std::string &_type, const std::string &_class) const {

  if (_pos >= expected_semantics.size())
    return false;

  if (expected_semantics[_pos].empty)
    return false;

  return expected_semantics[_pos].lex_type == _type && expected_semantics[_pos].lex_class == _class;
}

void SemanticSample::printPositon(size_t _pos) const {

  if (_pos >= expected_semantics.size() || expected_semantics[_pos].empty) {

    std::cout << "empty";
    return;
  }
  std::cout << expected_semantics[_pos].lex_class << "(" << expected_semantics[_pos].lex_type << ")";
}

bool SemanticSample::testFalsePositiveOk(const SemanticChain &_chain) const {
  
  size_t i = 0;
  for (auto p : _chain.positions) {

    if (!p.lexems.empty()) {

      auto& lex = p.lexems[0];
      if (! containsClass (i, lex.lex_type, lex.lex_class) ) {

        std::cout << "Test false positive. " << lex.lex_class << "(" << lex.lex_type << ")" << " pos: " << i << std::endl;
        std::cout << "Position must be ";
        printPositon(i);
        std::cout << std::endl;

        return false;
      }
    }
    ++i;
  }
  return true;
}

bool SemanticSample::isExpected(const SemanticChain &_chain) const {
  
  return testFalseNegativeOk(_chain) && testFalsePositiveOk(_chain);
}

void SemanticSample::print() const {
  
  std::cout << "SemanticSample size " << expected_semantics.size() << " |" << query << std::endl;

  int i = 0;
  for (auto p : expected_semantics) {

  	std::cout << "[" << i << "] ";
    if (p.empty) {

      std::cout << "empty\n";
    } else {

      std::cout << p.lex_class << "(" << p.lex_type << ")\n";
    }

    ++i;
  }
}

}
}
