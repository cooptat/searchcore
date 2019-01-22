#ifndef LINGUA_SEMANTIC_SAMPLE_H_
#define LINGUA_SEMANTIC_SAMPLE_H_

#include "SemanticChain.h"

#include <string>
#include <vector>

namespace style {
namespace lingua {

 struct SemanticSample {
    
    std::string query;

    struct ExpectedSemantic {

      bool empty;
      std::string lex_type;
      std::string lex_class;
    };

    std::vector<ExpectedSemantic> expected_semantics;

    bool testFalseNegativeOk(const SemanticChain &_chain) const;

    bool containsClass(size_t _pos, const std::string &_type, const std::string &_class) const;

    void printPositon(size_t _pos) const;

    bool testFalsePositiveOk(const SemanticChain &_chain) const;

    bool isExpected(const SemanticChain &_chain) const;

    void print() const;

  };

}
}

#endif
