#include <cppunit/extensions/HelperMacros.h>
#include "MacAddress.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME MacAddressTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_core);
    CPPUNIT_TEST(test_operators);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_core();
    void test_operators();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_core(){
    MacAddress *addr1 = new MacAddress();
    MacAddress *addr2 = new MacAddress((char *)"11:11:ab:cd:0f");
    MacAddress *addr3 = new MacAddress(0x2222abcd0f);
    
    CPPUNIT_ASSERT(0x0000000000 == addr1->toLong());
    CPPUNIT_ASSERT(0x1111abcd0f == addr2->toLong());
    CPPUNIT_ASSERT(0x2222abcd0f == addr3->toLong());
    CPPUNIT_ASSERT(addr3->toInt() == addr3->toLong());
    
    *addr1 = 0x0000abcd0f;
    CPPUNIT_ASSERT(0x0000abcd0f == addr1->toLong());
    
    delete addr1;
    delete addr2;
    delete addr3;
}

void FIXTURE_NAME::test_operators(){
    MacAddress *addr1 = new MacAddress();
    MacAddress *addr2 = new MacAddress((char *)"11:11:ab:cd:0f");
    MacAddress *addr3 = new MacAddress(0x1111abcd0f);
    
    CPPUNIT_ASSERT(*addr1 != *addr2);
    CPPUNIT_ASSERT(*addr2 == *addr3);
    
    delete addr1;
    delete addr2;
    delete addr3;
}
