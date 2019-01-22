#include "TestSemanticSort.h"
#include <iostream>
#include <fstream>

using namespace style::lingua;

void TestSemanticSort::setUp() {
  
  try {

    m_sex_det.reset(new WordDetJson(
      "SORT",
  	{
  	  "lex/sort.lex"
  	},
  	"lex/sort.bin",
  	5,
  	3,
    0.6f,
    0.7f
    ));

    m_sex_det->dump();

    std::vector<UnigramDetectorPtr> detectors;
    detectors.push_back( m_sex_det );

    m_sem.reset(new SemanticParser( detectors ));

    loadTestSamples("samples/sort.csv");

  } catch (std::exception &_ex) {
    
    std::cout << "TestSemanticSort::setUp ex " << _ex.what() << std::endl;
  }
}

void TestSemanticSort::testSemanticSamples() {
  
  std::cout << "TestSemanticSort::testSemanticSamples " << m_test_samples.size() << std::endl;
  for (auto sample : m_test_samples) {

     SemanticChain chain;
     m_sem->parse(sample.query, chain);

     if (!sample.isExpected(chain)) {

       std::cout << "test sample failed\n\n";// << sample.query << std::endl;

       sample.print();
       std::cout << std::endl;
       chain.print();
     }

    // chain.print();
  }
}
