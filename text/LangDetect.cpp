#include "LangDetect.h"
#include "cld2/public/compact_lang_det.h"
#include "cld2/public/encodings.h"


namespace searchcell {

Lang LangDetector::detect(const std::string &_str) {
	
	
	int flags = CLD2::kCLDFlagBestEffort;
	bool get_vector = false;
    bool is_plain_text = true;
     const char* tldhint = "";
     const CLD2::Encoding enchint = CLD2::UNKNOWN_ENCODING;
     const CLD2::Language langhint = CLD2::RUSSIAN;
     const CLD2::CLDHints cldhints = {NULL, tldhint, enchint, langhint};
     CLD2::Language language3[3];
     int percent3[3];
     double normalized_score3[3];
     CLD2::ResultChunkVector resultchunkvector;
     int text_bytes;
     bool is_reliable;
     int valid_prefix_bytes;

     CLD2::Language lang_detected = CLD2::ExtDetectLanguageSummaryCheckUTF8(
                             _str.c_str(),
                             _str.size(),
                             is_plain_text,
                             &cldhints,
                             flags,
                             language3,
                             percent3,
                             normalized_score3,
                             get_vector ? &resultchunkvector : NULL,
                             &text_bytes,
                             &is_reliable,
   &valid_prefix_bytes);
   
	////
	
	//int valid_prefix_bytes;
	//bool is_reliable;
//	CLD2::Language lang =
	//		CLD2::DetectLanguageCheckUTF8(_str.c_str(), _str.size(), 1, &is_reliable, &valid_prefix_bytes);
	
  // std::cout << "LangDetector::detect valid_prefix_bytes " << valid_prefix_bytes << " is_reliable " << is_reliable << " flags: " << flags <<  std::endl;
	
	if (lang_detected == CLD2::ENGLISH) {
		return EN;
	}
	if (lang_detected == CLD2::RUSSIAN) {
		return RU;
	}
	return UNKNOWN_LANG;
}

std::ostream& operator<<(std::ostream& os, const Lang& lang) {
	
	if (lang == EN)
		os << "EN";
	if (lang == RU)
		os << "RU";
	if (lang == UNKNOWN_LANG)
		os << "UNKNOWN_LANG";
		
    return os;
}
	
};
