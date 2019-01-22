#include "TestSemanticSex.h"
#include <iostream>
#include <fstream>

using namespace style::lingua;

void TestSemanticSex::setUp() {
  
  try {

    m_sex_det.reset(new WordDetJson(
      "SEX",
  	{
  	  "lex/sex.lex"
  	},
  	"lex/sex.bin",
  	5,
  	3,
    0.6f,
    0.7f
    ));

    m_sex_det->dump();

    std::vector<UnigramDetectorPtr> detectors;
    detectors.push_back( m_sex_det );

    m_sem.reset(new SemanticParser( detectors ));

    loadTestSamples("samples/sex.csv");

  } catch (std::exception &_ex) {
    
    std::cout << "TestSemanticSex::setUp ex " << _ex.what() << std::endl;
  }
}

void TestSemanticSex::testSemanticSamples() {
  
  std::cout << "TestSemanticSex::testSemanticSamples " << m_test_samples.size() << std::endl;
  for (auto sample : m_test_samples) {

     SemanticChain chain;
     m_sem->parse(sample.query, chain);

     if (!sample.isExpected(chain)) {

       std::cout << "test sample failed\n\n";// << sample.query << std::endl;

       sample.print();
       std::cout << std::endl;
       chain.print();
     }

     //chain.print();
  }
}
/*
void TestSemanticParser::testSemanticOne() {
  
  try {
    SemanticChain chain;

    m_sem->parse("девушки блондинки москвы ", chain);

    chain.print();

    TS_ASSERT( chain.containsClass(0, "SEX", "F") == true );

  } catch (std::exception &_ex) {
    
    std::cout << "TestSemanticParser::testSemantic ex " << _ex.what() << std::endl;
  }
}
*/
