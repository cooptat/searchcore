#ifndef TEST_SEMANTIC_BASE_H_
#define TEST_SEMANTIC_BASE_H_

#include "SemanticParser.h"
#include "SemanticSample.h"
#include "WordDetJson.h"

#include <exception>
#include <iostream>

class TestSemanticBase {

protected:

  style::lingua::WordDetJsonPtr m_sex_det;
  style::lingua::SemanticParserPtr m_sem;

  std::vector<style::lingua::SemanticSample> m_test_samples;

  void loadTestSamples(const std::string &_fn);


public:
  

};

#endif
