#ifndef TEST_SEMANTIC_SEX_H_
#define TEST_SEMANTIC_SEX_H_

#include <cxxtest/TestSuite.h>
#include "TestSemanticBase.h"

class TestSemanticSex : public TestSemanticBase, public CxxTest::TestSuite {
public:

  void setUp();
  void testSemanticSamples();
};

#endif
