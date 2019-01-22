#ifndef TEST_SEMANTIC_SORT_H_
#define TEST_SEMANTIC_SORT_H_

#include <cxxtest/TestSuite.h>
#include "TestSemanticBase.h"

class TestSemanticSort : public TestSemanticBase, public CxxTest::TestSuite {
public:

  void setUp();
  void testSemanticSamples();
};

#endif
