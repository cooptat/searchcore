#include "WordDetJson.h"
#include <iostream>
#include <fstream>
#include <json/json.h>

namespace style {
namespace lingua {

WordDetJson::WordDetJson(
	const std::string &_detector_type,
	const std::vector<std::string> &_lexem_meta_tuned_fn,
	const std::string &_speller_dump_fn,
	unsigned _default_spell_distance_limit,
	unsigned _default_spell_distance_ask_correct,
	float _default_spell_distnance_ratio,
	float _default_spell_ask_distnance_ratio
):
  WordDet::WordDet(
	  _detector_type,
	  _speller_dump_fn,
	  _default_spell_distance_limit,
	  _default_spell_distance_ask_correct,
	  _default_spell_distnance_ratio,
	  _default_spell_ask_distnance_ratio) {
  
	for (auto f : _lexem_meta_tuned_fn) {
		
		loadLexemMeta(f);
	}
}

void WordDetJson::loadLexemMeta(const std::string &_fn) {
  
	std::ifstream f(_fn);
	
	Json::Value js;
	Json::Reader reader;
	
  if (!reader.parse(f, js)) {
  	
		throw std::invalid_argument("WordDetJson::loadLexemMeta could not parse " + _fn);
  }
	
	for (auto class_it = js.begin(); class_it != js.end(); ++class_it) {
		
		StringUtf8 lex_class = StringUtf8( class_it.key().asString() );
		
		for (auto meta_it = class_it->begin(); meta_it != class_it->end(); ++meta_it) {
			
			auto word = StringUtf8( meta_it.key().asString() );
			
			StringUtf8 replace;
			bool use_stem = true;
			bool use_spell = true;
			bool is_public = false;
			unsigned ask_correct_distance = m_default_spell_distance_ask_correct;
			
			for (auto f_it = meta_it->begin(); f_it != meta_it->end(); ++f_it) {
				
				std::string field_name = f_it.key().asString();
				
				if (field_name == "replace") {
					
					replace = StringUtf8( std::string( f_it->asString() ) );
					continue;
				}
				
				if (field_name == "use_stem") {
				
				  use_stem = f_it->asBool();
					continue;
			  }
				
				if (field_name == "use_spell") {
				
				  use_spell = f_it->asBool();
					continue;
			  }
				
				if (field_name == "ask_correct_distance") {
				
					ask_correct_distance = f_it->asInt();
					continue;
			  	}

			  	if (field_name == "public") {
				
				  is_public = f_it->asBool();
					continue;
			  }
			}
			
			LexemMeta lexem_meta { word, replace, lex_class, use_stem, use_spell, ask_correct_distance, is_public };
			compileLexem(lexem_meta);
		}
	}
}

WordDetJson::~WordDetJson() { }

}
}
