#include "TestSemanticBase.h"
#include <iostream>
#include <fstream>

using namespace style::lingua;

void TestSemanticBase::loadTestSamples(const std::string &_fn) {
  
  std::ifstream in (_fn);

  while (!in.eof()) {

  	std::string _line;
  	std::getline(in, _line);

    if (in.eof())
      break;

  	StringUtf8 line(_line);

  	line.fixUtf8();

  	if (line.size() < 3)
  	  continue;

  	StringUtf8 query;
  	SemanticSample sample;

  	auto tokens = StringUtf8(line).split(44); // comma
  	int i = 0;
  	for (auto token : tokens) {
      
      //std::cout << "comma token |" << token << "|" << std::endl;

      if (token.empty()) {
        //++i;
        sample.expected_semantics.push_back({ true, "", "" });
        continue;
      }

      if (i == 0) {
      	query = token;

      	sample.query = query;
      	++i;
      	continue;
      } 

      auto lex = token.split(32); // space

      if (lex.size() < 2) {
      	sample.expected_semantics.push_back({ true, "", "" });
        ++i;
        continue;
      }

      auto lex_type = lex[0];
      auto lex_class = lex[1];

      if (!lex_type.empty() && !lex_class.empty()) {
        sample.expected_semantics.push_back({ false, lex_type, lex_class });
        ++i;
        continue;
      }

      sample.expected_semantics.push_back({ true, "", "" });
      ++i;
  	} 

    m_test_samples.push_back(sample);
  }
}
