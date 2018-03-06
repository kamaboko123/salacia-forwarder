#include <cppunit/extensions/HelperMacros.h>
#include <cstring>
#include "IPAddress.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME IPAddressTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_ipaddress_core);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_ipaddress_core();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_ipaddress_core(){
    IPAddress *addr1 = new IPAddress();
    IPAddress *addr2 = new IPAddress((char *)"172.16.101.1");
    
    //[test]isSet
    CPPUNIT_ASSERT_EQUAL(false, addr1->isSet());
    CPPUNIT_ASSERT_EQUAL(true, addr2->isSet());
    
    //[test]touInt
    CPPUNIT_ASSERT_EQUAL(0, (int)addr1->touInt());
    CPPUNIT_ASSERT_EQUAL((int)0xAC106501, (int)addr2->touInt());
    
    //[test]toStr
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0", addr1->toStr()));
    CPPUNIT_ASSERT_EQUAL(0, strcmp("172.16.101.1", addr2->toStr()));
    
    //[test]set
    addr1->set((char *)"10.0.0.1");
    CPPUNIT_ASSERT_EQUAL(true, addr1->isSet());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("10.0.0.1", addr1->toStr()));
    
    addr1->set((char *)"10.0.0.1", false);
    CPPUNIT_ASSERT_EQUAL(false, addr1->isSet());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("10.0.0.1", addr1->toStr()));
    
    addr2->set(0xc0a80101);
    CPPUNIT_ASSERT_EQUAL(0, strcmp("192.168.1.1", addr2->toStr()));
    
    delete addr1;
    delete addr2;
}
