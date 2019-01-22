#ifndef WORD_DETECTOR_JSON_H_
#define WORD_DETECTOR_JSON_H_

#include "WordDet.h"

namespace style {
namespace lingua {

class WordDetJson : public WordDet {

  void loadLexemMeta(const std::string &_fn);
	
public:

  WordDetJson(
		const std::string &_detector_type,
		const std::vector<std::string> &_lexem_meta_tuned_fn,
		const std::string &_speller_dump_fn,
		unsigned _default_spell_distance_limit = 5,
		unsigned _default_spell_distance_ask_correct = 4,
		float _default_spell_distnance_ratio = 0.25,
	    float _default_spell_ask_distnance_ratio = 0.20
		);
	
  virtual ~WordDetJson();
};

typedef boost::shared_ptr<WordDetJson> WordDetJsonPtr;

}
}

#endif
