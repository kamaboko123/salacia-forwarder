#include <cppunit/extensions/HelperMacros.h>
#include "comlib.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME ComlibTest

using namespace comlib;

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_strlen);
    CPPUNIT_TEST(test_isdigit);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_strlen();
    void test_isdigit();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp(){}
void FIXTURE_NAME::tearDown(){}

void FIXTURE_NAME::test_strlen(){
    CPPUNIT_ASSERT_EQUAL(6, strlen((char *)"string"));
}

void FIXTURE_NAME::test_isdigit(){
    CPPUNIT_ASSERT_EQUAL(true, isdigit('1'));
    CPPUNIT_ASSERT_EQUAL(true, isdigit('9'));
    CPPUNIT_ASSERT_EQUAL(false, isdigit('1'));
}
