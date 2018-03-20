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
    CPPUNIT_TEST(test_ipaddress_validation);
    CPPUNIT_TEST(test_ipaddress_static);
    CPPUNIT_TEST(test_ipnetmask_core);
    CPPUNIT_TEST(test_ipnetwork_core);
    CPPUNIT_TEST(test_ipnetwork_static);
    CPPUNIT_TEST_SUITE_END();

private:
    void copy_constructor_test(IPAddress target, uint32_t expect_int_addr);
    void copy_constructor_test(IPNetwork target, IPNetwork &expect_net);
    
public:
    void setUp();
    void tearDown();
protected:
    void test_ipaddress_core();
    void test_ipaddress_validation();
    void test_ipaddress_static();
    void test_ipnetmask_core();
    void test_ipnetwork_core();
    void test_ipnetwork_static();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::copy_constructor_test(IPAddress target, uint32_t expect_int_addr){
    CPPUNIT_ASSERT_EQUAL(expect_int_addr, target.touInt());
}

void FIXTURE_NAME::copy_constructor_test(IPNetwork target, IPNetwork &expect_net){
    CPPUNIT_ASSERT_EQUAL(expect_net.isValid(), target.isValid());
    CPPUNIT_ASSERT_EQUAL(expect_net.getNetaddr().touInt(), target.getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(expect_net.getNetmask().touInt(), target.getNetmask().touInt());
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

void FIXTURE_NAME::test_ipaddress_validation(){
    CPPUNIT_ASSERT_THROW(IPAddress("10.0.0.1000"), sfwdr::Exception::InvalidIPAddress);
    CPPUNIT_ASSERT_THROW(IPAddress("999.999.999.999"), sfwdr::Exception::InvalidIPAddress);
    CPPUNIT_ASSERT_THROW(IPAddress("-10.0.0.0"), sfwdr::Exception::InvalidIPAddress);
    CPPUNIT_ASSERT_THROW(IPAddress("10.0.0.0/"), sfwdr::Exception::InvalidIPAddress);
    CPPUNIT_ASSERT_THROW(IPAddress("a"), sfwdr::Exception::InvalidIPAddress);
    
    //leak check
    IPAddress *addr;
    try{
        addr = new IPAddress("999.999.999.999");
        delete(addr);
    }
    catch(sfwdr::Exception::InvalidIPAddress &e){}
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

void FIXTURE_NAME::test_ipnetmask_core(){
    IPNetmask *mask1 = new IPNetmask();
    IPNetmask *mask2 = new IPNetmask((char *)"255.255.255.0");
    IPNetmask *mask3 = new IPNetmask((char *)"255.255.255.255");
    IPNetmask *mask4 = new IPNetmask(0xFFFF0000);
    
    //[test] not set
    CPPUNIT_ASSERT_EQUAL(false, mask1->isSet());
    CPPUNIT_ASSERT_EQUAL(false, mask1->isValid());
    CPPUNIT_ASSERT_EQUAL(-1, mask1->getLength());
    
    //[test] /24
    CPPUNIT_ASSERT_EQUAL(true, mask2->isSet());
    CPPUNIT_ASSERT_EQUAL(true, mask2->isValid());
    CPPUNIT_ASSERT_EQUAL(24, mask2->getLength());
    
    //[test] /32
    CPPUNIT_ASSERT_EQUAL(true, mask3->isSet());
    CPPUNIT_ASSERT_EQUAL(true, mask3->isValid());
    CPPUNIT_ASSERT_EQUAL(32, mask3->getLength());
    
    //[test] /16 with constructor(uint32_t)
    CPPUNIT_ASSERT_EQUAL(true, mask4->isSet());
    CPPUNIT_ASSERT_EQUAL(true, mask4->isValid());
    CPPUNIT_ASSERT_EQUAL(16, mask4->getLength());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("255.255.0.0", mask4->toStr()));
    
    //[test] /0
    CPPUNIT_ASSERT_EQUAL(0, mask1->set((uint32_t)0));
    CPPUNIT_ASSERT_EQUAL(true, mask1->isSet());
    CPPUNIT_ASSERT_EQUAL(true, mask1->isValid());
    CPPUNIT_ASSERT_EQUAL(0, mask1->getLength());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0", mask1->toStr()));
    
    CPPUNIT_ASSERT_EQUAL(0, mask2->set((char *)"0.0.0.0"));
    CPPUNIT_ASSERT_EQUAL(true, mask2->isSet());
    CPPUNIT_ASSERT_EQUAL(true, mask2->isValid());
    CPPUNIT_ASSERT_EQUAL(0, mask2->getLength());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, mask2->touInt());
    
    CPPUNIT_ASSERT_EQUAL(0, mask3->setLength(0));
    CPPUNIT_ASSERT_EQUAL(true, mask3->isSet());
    CPPUNIT_ASSERT_EQUAL(true, mask3->isValid());
    CPPUNIT_ASSERT_EQUAL(0, mask3->getLength());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, mask3->touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0", mask3->toStr()));
    
    //[test] /23
    CPPUNIT_ASSERT_EQUAL(23, mask1->setLength(23));
    CPPUNIT_ASSERT_EQUAL(true, mask1->isSet());
    CPPUNIT_ASSERT_EQUAL(true, mask1->isValid());
    CPPUNIT_ASSERT_EQUAL(23, mask1->getLength());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0xfffffe00, mask1->touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("255.255.254.0", mask1->toStr()));
    
    //[test] invalid
    CPPUNIT_ASSERT_EQUAL(-1, mask1->setLength(33));
    CPPUNIT_ASSERT_EQUAL(true, mask1->isSet());
    CPPUNIT_ASSERT_EQUAL(false, mask1->isValid());
    CPPUNIT_ASSERT_EQUAL(-1, mask1->getLength());
    
    CPPUNIT_ASSERT_EQUAL(-1, mask2->set((char *)"111.111.111.111"));
    CPPUNIT_ASSERT_EQUAL(true, mask2->isSet());
    CPPUNIT_ASSERT_EQUAL(false, mask2->isValid());
    CPPUNIT_ASSERT_EQUAL(-1, mask2->getLength());
    
    //[test]/8 with set(uint32_t)
    CPPUNIT_ASSERT_EQUAL(8, mask3->set(0xff000000));
    CPPUNIT_ASSERT_EQUAL(true, mask3->isSet());
    CPPUNIT_ASSERT_EQUAL(true, mask3->isValid());
    CPPUNIT_ASSERT_EQUAL(8, mask3->getLength());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("255.0.0.0", mask3->toStr()));
    
    delete mask1;
    delete mask2;
    delete mask3;
    delete mask4;
}

void FIXTURE_NAME::test_ipnetwork_core(){
    IPNetwork *net;
    
    //[test] not set
    net = new IPNetwork();
    CPPUNIT_ASSERT_EQUAL(false, net->isValid());
    CPPUNIT_ASSERT_EQUAL((uint32_t)1, net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, net->getNetmask().touInt());
    delete net;
    
    //[test] 192.168.0.0/24
    net = new IPNetwork((char *)"192.168.0.0/24");
    CPPUNIT_ASSERT_EQUAL(true, net->isValid());
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"192.168.0.0").touInt(), net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"255.255.255.0").touInt(), net->getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("192.168.0.0/24", net->toStr()));
    delete net;
    
    IPAddress net_addr((char *)"192.168.1.0");
    net = new IPNetwork(net_addr, 24);
    CPPUNIT_ASSERT_EQUAL(true, net->isValid());
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"192.168.1.0").touInt(), net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"255.255.255.0").touInt(), net->getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("192.168.1.0/24", net->toStr()));
    delete net;
    
    //[test] 192.168.1.0/23
    net = new IPNetwork((char *)"192.168.1.0/23");
    CPPUNIT_ASSERT_EQUAL(false, net->isValid());
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"192.168.1.0").touInt(), net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"255.255.254.0").touInt(), net->getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("", net->toStr()));
    delete net;
    
    //[test] 0.0.0.0/0
    net = new IPNetwork((char *)"0.0.0.0/0");
    CPPUNIT_ASSERT_EQUAL(true, net->isValid());
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"0.0.0.0").touInt(), net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"0.0.0.0").touInt(), net->getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0/0", net->toStr()));
    delete net;
    
    //[test] copy constructor
    net = new IPNetwork((char *)"192.168.1.0/23");
    copy_constructor_test(*net, *net);
    IPNetwork net_copied = *net;
    CPPUNIT_ASSERT_EQUAL(net->isValid(), net_copied.isValid());
    CPPUNIT_ASSERT_EQUAL(net->getNetaddr().touInt(), net_copied.getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(net->getNetmask().touInt(), net_copied.getNetmask().touInt());
    delete net;
}

void FIXTURE_NAME::test_ipnetwork_static(){
    //validPrefixFormat (just format check, not prefix validate)
    CPPUNIT_ASSERT_EQUAL(true, IPNetwork::validPrefixFormat((char *)"192.168.0.0/23"));
    CPPUNIT_ASSERT_EQUAL(true, IPNetwork::validPrefixFormat((char *)"192.168.0.0/24"));
    CPPUNIT_ASSERT_EQUAL(true, IPNetwork::validPrefixFormat((char *)"0.0.0.0/0"));
    CPPUNIT_ASSERT_EQUAL(true, IPNetwork::validPrefixFormat((char *)"1.1.1.1/32"));
    CPPUNIT_ASSERT_EQUAL(false, IPNetwork::validPrefixFormat((char *)"111.111.111.1111/32"));
    CPPUNIT_ASSERT_EQUAL(false, IPNetwork::validPrefixFormat((char *)"1.1.1.1/33"));
    CPPUNIT_ASSERT_EQUAL(false, IPNetwork::validPrefixFormat((char *)"1.1.1.1/-1"));
    
    
}
