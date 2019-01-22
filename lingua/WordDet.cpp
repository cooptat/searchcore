#include "WordDet.h"

namespace style {
namespace lingua {

WordDet::WordDet(
	  const std::string &_detector_type,
		const std::string &_speller_dump_fn,
	  unsigned _default_spell_distance_limit,
	  unsigned _default_spell_distance_ask_correct,
	  float _default_spell_distnance_ratio,
	  float _default_spell_ask_distnance_ratio
):
	UnigramDetector::UnigramDetector(_detector_type),
	
	m_speller_dump_fn(_speller_dump_fn),
    m_default_spell_distance_limit(_default_spell_distance_limit),
	m_default_spell_distance_ask_correct(_default_spell_distance_ask_correct),
	m_default_spell_distnance_ratio(_default_spell_distnance_ratio),
	m_default_spell_ask_distnance_ratio(_default_spell_ask_distnance_ratio) {		
}

WordDet::~WordDet() {
}

void WordDet::compileLexem(const LexemMeta &_lex_meta) {
	
	m_lexem_meta[ _lex_meta.word ] = _lex_meta;

	if (_lex_meta.use_stem) {
	
		auto& word = _lex_meta.word;
		auto stem = m_stemmer.stem(_lex_meta.word);
		if (!stem.empty()) {
		
			auto distance = static_cast<int>(word.size() - stem.size()); // FIX DISTANCE NOT UNICODE
			m_stems_class[ stem ] = { word, _lex_meta.lex_class, distance };
		}
	}

	if (_lex_meta.use_spell) {
	
		m_speller.addWord(1, _lex_meta.word);	
	}
}

void WordDet::compileLexems(const std::vector<LexemMeta> &_lexem_meta) {

  for (auto m : _lexem_meta) {
  
    compileLexem( m );
  }
}

int WordDet::detect(const Token &_token, Lexem &_lexem) const {
  
   //_lexem.recognized = false;

	if (m_lexem_meta.empty())
		return Lexem::max_distance;
  {
	  auto it = m_lexem_meta.find(_token.exact);
	  if (it != m_lexem_meta.end()) {

		_lexem.recognized = true;
	  	_lexem.distance = 0;
	  	_lexem.token = _token;
	  	_lexem.corrected = false;
		 _lexem.ask_to_replace = false;

		 if (!it->second.replace.empty()) {

		   _lexem.corrected = true;
		   _lexem.corrected_token.exact = it->second.replace;
		 }

		_lexem.lex_type = m_lex_type;
		_lexem.lex_class = it->second.lex_class;

	  	return 0;
	  }

  }
	  {
	  // stems 
	  auto sit = m_stems_class.find(_token.stem);
	  if (sit != m_stems_class.end()) {

			const StemClassDistance & stem_class = sit->second;
			auto it = m_lexem_meta.find(stem_class.original);

			_lexem.recognized = true;
	  	_lexem.distance = stem_class.distance;
			
	  	_lexem.token = _token;
	  	_lexem.corrected = false;
		  _lexem.ask_to_replace = false;

		  if (!it->second.replace.empty()) {

		   _lexem.corrected = true;
		   _lexem.corrected_token.exact = it->second.replace;
		 }

		  _lexem.lex_type = m_lex_type;
		  _lexem.lex_class = stem_class.lex_class;

	  	return stem_class.distance;
	  }
  }

  {
  // spellchecker
	auto matches = m_speller.match( _token.exact, m_default_spell_distance_limit);
	if (!matches.empty()) {
		
		auto word = StringUtf8(matches[0].word);

		if (word.size() == 0)
		  return Lexem::max_distance;

		float distance_ratio = float(matches[0].distance) / float(word.size());
		if (distance_ratio > m_default_spell_distnance_ratio)
		  return Lexem::max_distance;
		
		auto it = m_lexem_meta.find(word);
	  if (it != m_lexem_meta.end()) {

		_lexem.recognized = true;
	  	_lexem.distance = matches[0].distance;
	  	_lexem.token.exact = word;
		_lexem.token.stem = word;
			
	  	_lexem.corrected = true;
			
		_lexem.ask_to_replace = false;

			if (distance_ratio > m_default_spell_ask_distnance_ratio || _lexem.distance > it->second.ask_correct_distance)
		    _lexem.ask_to_replace = true;

		  _lexem.lex_type = m_lex_type;
		  _lexem.lex_class = it->second.lex_class;
			
			return matches[0].distance;
		}
	}
  }

  return Lexem::max_distance;
}

void WordDet::exportPublicLexems( std::map<std::string, std::vector<std::string> > &_public_lex ) const {

	for (auto it : m_lexem_meta) {
		auto lex = it.second;

		if (lex.is_public)
			_public_lex[ lex.lex_class ].push_back( lex.word );
	}
}

void WordDet::dump() {
	
	m_speller.dump( m_speller_dump_fn );
}

void WordDet::load() {
	
	m_speller.load( m_speller_dump_fn );
}
	
}
}
