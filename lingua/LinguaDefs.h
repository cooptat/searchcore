#ifndef STYLESEARCH_LINGUA_DEFS_H_
#define STYLESEARCH_LINGUA_DEFS_H_

#include <vector>
#include "searchcell/strings/StringUtf8.h"

namespace style {
namespace lingua {

typedef typename searchcell::StringUtf8 StringUtf8;

struct Token {
  
  StringUtf8 exact;
  StringUtf8 stem;
};

}
}

#endif
