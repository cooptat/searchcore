#ifndef STYLESEARCH_WORD_DETECTOR_H_
#define STYLESEARCH_WORD_DETECTOR_H_

#include "UnigramDetector.h"
#include "searchcell/strings/Stemmer.h"
#include "searchcell/strings/Speller.h"

#include <unordered_map>
#include <map>

namespace style {
namespace lingua {

class WordDet : public UnigramDetector {
protected:
	
	struct StemClassDistance {
		
		StringUtf8 original;
		std::string lex_class;
		
		int distance;
	};
	
	struct LexemMeta {
	
		StringUtf8 word;
		StringUtf8 replace;
	
		std::string lex_class;
	
		bool use_stem;
		bool use_spell;
		unsigned ask_correct_distance;
		bool is_public;
	};

    std::unordered_map<StringUtf8, LexemMeta> m_lexem_meta;
	std::unordered_map<StringUtf8, StemClassDistance> m_stems_class;
	
	searchcell::Stemmer m_stemmer;
	searchcell::Speller m_speller;
	
	std::string m_speller_dump_fn;
	
	unsigned m_default_spell_distance_limit = 5;
	unsigned m_default_spell_distance_ask_correct = 4;

	float m_default_spell_distnance_ratio = 0.25;
	float m_default_spell_ask_distnance_ratio = 0.20;
	
	void compileLexem(const LexemMeta &_lex_meta);
  void compileLexems(const std::vector<LexemMeta> &_lexem_meta);

public:
	
	WordDet(
		const std::string &_detector_type,
		const std::string &_speller_dump_fn,
	  unsigned m_default_spell_distance_limit = 5,
	  unsigned m_default_spell_distance_ask_correct = 4,
	  float _default_spell_distnance_ratio = 0.25,
	  float _default_spell_ask_distnance_ratio = 0.20
	);
	
	virtual ~WordDet();
	
	virtual int detect(const Token &_token, Lexem &_lexem) const;
	
	virtual void exportPublicLexems( std::map<std::string, std::vector<std::string> > &_public_lex ) const;

	virtual void dump();
	virtual void load();
};

}
}

#endif
