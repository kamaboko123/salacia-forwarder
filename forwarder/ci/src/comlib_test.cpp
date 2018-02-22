#include <cppunit/extensions/HelperMacros.h>
#include <cstring>
#include "comlib.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME ComlibTest

namespace clib = comlib;

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_strncpy);
    CPPUNIT_TEST(test_strlen);
    CPPUNIT_TEST(test_isdigit);
    CPPUNIT_TEST(test_atoi);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_strncpy();
    void test_strlen();
    void test_isdigit();
    void test_atoi();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp(){}
void FIXTURE_NAME::tearDown(){}

void FIXTURE_NAME::test_strncpy(){
    char *str;
    
    //配列の動的確保時に()つけると0で初期化されるんですが、これってC++規格？
    str =  new char[16]();
    CPPUNIT_ASSERT_EQUAL(true, str == clib::strncpy(str, (char *)"abc", 3));
    CPPUNIT_ASSERT_EQUAL(0, strcmp(str, "abc"));
    delete[] str;
    
    str = new char[16]();
    CPPUNIT_ASSERT_EQUAL(true, str == clib::strncpy(str, (char *)"abc", 2));
    CPPUNIT_ASSERT_EQUAL(0, strcmp(str, "ab"));
    delete[] str;
}

void FIXTURE_NAME::test_strlen(){
    CPPUNIT_ASSERT_EQUAL(6, clib::strlen((char *)"string"));
}

void FIXTURE_NAME::test_isdigit(){
    CPPUNIT_ASSERT_EQUAL(true, clib::isdigit('1'));
    CPPUNIT_ASSERT_EQUAL(true, clib::isdigit('9'));
    CPPUNIT_ASSERT_EQUAL(false, clib::isdigit('a'));
}

void FIXTURE_NAME::test_atoi(){
    CPPUNIT_ASSERT_EQUAL(clib::atoi((char *)"1234567890"), 1234567890);
    CPPUNIT_ASSERT_EQUAL(clib::atoi((char *)"12345a67890"), 12345);
    CPPUNIT_ASSERT_EQUAL(clib::atoi((char *)"a1234567890"), 0);
}

