#include <cxxtest/TestSuite.h>
#include "searchcell/strings/FixString.h"

class FixStringTests : public CxxTest::TestSuite {
	
	typedef searchcell::FixStringT<uint8_t, 20> FixString;
	
public:

    void testConstructor();
};
