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
    CPPUNIT_TEST(test_ipaddress_static);
    CPPUNIT_TEST_SUITE_END();

private:
    void copy_constructor_test(IPAddress target, uint32_t expect_int_addr);
    
public:
    void setUp();
    void tearDown();
protected:
    void test_ipaddress_core();
    void test_ipaddress_static();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::copy_constructor_test(IPAddress target, uint32_t expect_int_addr){
    CPPUNIT_ASSERT_EQUAL(expect_int_addr, target.touInt());
}

void FIXTURE_NAME::test_ipaddress_core(){
    IPAddress *addr1 = new IPAddress();
    IPAddress *addr2 = new IPAddress((char *)"172.16.101.1");
    
    //[test]isSet
    CPPUNIT_ASSERT_EQUAL(false, addr1->isSet());
    CPPUNIT_ASSERT_EQUAL(true, addr2->isSet());
    
    //[test]touInt
    CPPUNIT_ASSERT_EQUAL(0, (int)addr1->touInt());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0xAC106501, addr2->touInt());
    
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
    
    //copy constructor and operators
    copy_constructor_test(*addr2, 0xc0a80101);
    
    IPAddress addr_copied = *addr2;
    CPPUNIT_ASSERT_EQUAL((uint32_t)0xc0a80101, addr_copied.touInt());
    
    delete addr1;
    delete addr2;
    
}

void FIXTURE_NAME::test_ipaddress_static(){
    char buf[32];
    
    //IPAddress:uitoip()
    CPPUNIT_ASSERT(nullptr == IPAddress::uitoip(0xAC106501, buf, IP_ADDR_STR_LEN - 1));
    CPPUNIT_ASSERT(buf == IPAddress::uitoip(0xAC106501, buf, 32));
    CPPUNIT_ASSERT_EQUAL(0, strcmp("172.16.101.1", buf));
    
    //IPAddress:iptoui()
    CPPUNIT_ASSERT_EQUAL(0xAC106501, IPAddress::iptoui((char *)"172.16.101.1"));
}

