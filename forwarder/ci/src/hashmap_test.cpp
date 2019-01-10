#include <cstring>
#include <cppunit/extensions/HelperMacros.h>
#include "HashMap.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME HashMapTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_core);
    CPPUNIT_TEST_SUITE_END();

private:
public:
    void setUp();
    void tearDown();
protected:
    void test_core();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_core(){
}

