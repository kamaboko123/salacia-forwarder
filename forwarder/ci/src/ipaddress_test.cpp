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
    CPPUNIT_TEST(test_ipnetmask_validation);
    CPPUNIT_TEST(test_ipnetwork_core);
    CPPUNIT_TEST(test_ipnetwork_validation);
    CPPUNIT_TEST_SUITE_END();

private:
    void copy_constructor_test(IPAddress target, IPAddress &expect);
    void copy_constructor_test(IPNetmask target, IPNetmask &expect);
    void copy_constructor_test(IPNetwork target, IPNetwork &expect);
    
public:
    void setUp();
    void tearDown();
protected:
    void test_ipaddress_core();
    void test_ipaddress_validation();
    void test_ipaddress_static();
    void test_ipnetmask_core();
    void test_ipnetmask_validation();
    void test_ipnetwork_core();
    void test_ipnetwork_validation();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::copy_constructor_test(IPAddress target,  IPAddress &expect){
    CPPUNIT_ASSERT_EQUAL(expect.touInt(), target.touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp(expect.toStr(), target.toStr()));
}

void FIXTURE_NAME::copy_constructor_test(IPNetmask target,  IPNetmask &expect){
    CPPUNIT_ASSERT_EQUAL(expect.touInt(), target.touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp(expect.toStr(), target.toStr()));
}

void FIXTURE_NAME::copy_constructor_test(IPNetwork target, IPNetwork &expect){
    CPPUNIT_ASSERT_EQUAL(expect.getNetaddr().touInt(), target.getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(expect.getNetmask().touInt(), target.getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp(expect.toStr(), target.toStr()));
}

void FIXTURE_NAME::test_ipaddress_core(){
    IPAddress *addr1 = new IPAddress();
    IPAddress *addr2 = new IPAddress((char *)"172.16.101.1");
    IPAddress *addr3 = new IPAddress(0xc0a80101);
    
    //[test]touInt
    CPPUNIT_ASSERT_EQUAL(0, (int)addr1->touInt());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0xAC106501, addr2->touInt());
    
    //[test]toStr
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0", addr1->toStr()));
    CPPUNIT_ASSERT_EQUAL(0, strcmp("192.168.1.1", addr3->toStr()));
    
    //copy constructor and operators
    copy_constructor_test(*addr3, *addr3);
    
    IPAddress addr_copied = *addr3;
    CPPUNIT_ASSERT_EQUAL(addr3->touInt(), addr_copied.touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp(addr3->toStr(), addr_copied.toStr()));
    
    delete addr1;
    delete addr2;
    delete addr3;
    
    //leak check
    try{
        addr1 = new IPAddress((char *)"999.999.999.999");
    }
    catch(sfwdr::Exception::InvalidIPAddress &e){}
}

void FIXTURE_NAME::test_ipaddress_validation(){
    CPPUNIT_ASSERT_THROW(IPAddress((char *)"10.0.0.1000"), sfwdr::Exception::InvalidIPAddress);
    CPPUNIT_ASSERT_THROW(IPAddress((char *)"999.999.999.999"), sfwdr::Exception::InvalidIPAddress);
    CPPUNIT_ASSERT_THROW(IPAddress((char *)"-10.0.0.0"), sfwdr::Exception::InvalidIPAddress);
    CPPUNIT_ASSERT_THROW(IPAddress((char *)"10.0.0.0/"), sfwdr::Exception::InvalidIPAddress);
    CPPUNIT_ASSERT_THROW(IPAddress((char *)"a"), sfwdr::Exception::InvalidIPAddress);
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
    
    //[test] default
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0", mask1->toStr()));
    CPPUNIT_ASSERT_EQUAL(0, mask1->getLength());
    
    //[test] /24
    CPPUNIT_ASSERT_EQUAL(24, mask2->getLength());
    
    //[test] /32
    CPPUNIT_ASSERT_EQUAL(32, mask3->getLength());
    
    //[test] /16 with constructor(uint32_t)
    CPPUNIT_ASSERT_EQUAL(16, mask4->getLength());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("255.255.0.0", mask4->toStr()));
    
    //[test] /0
    CPPUNIT_ASSERT_EQUAL(0, mask1->set((uint32_t)0));
    CPPUNIT_ASSERT_EQUAL(0, mask1->getLength());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0", mask1->toStr()));
    
    CPPUNIT_ASSERT_EQUAL(0, mask2->set((char *)"0.0.0.0"));
    CPPUNIT_ASSERT_EQUAL(0, mask2->getLength());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, mask2->touInt());
    
    CPPUNIT_ASSERT_EQUAL(0, mask3->setLength(0));
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, mask3->touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0", mask3->toStr()));
    
    //[test] /23
    CPPUNIT_ASSERT_EQUAL(23, mask1->setLength(23));
    CPPUNIT_ASSERT_EQUAL(23, mask1->getLength());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0xfffffe00, mask1->touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("255.255.254.0", mask1->toStr()));
    
    //[test]/8 with set(uint32_t)
    CPPUNIT_ASSERT_EQUAL(8, mask3->set(0xff000000));
    CPPUNIT_ASSERT_EQUAL(8, mask3->getLength());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("255.0.0.0", mask3->toStr()));
    
    delete mask1;
    delete mask2;
    delete mask3;
    delete mask4;
    
    //copy constructor and operators
    mask1 = new IPNetmask("255.255.255.252");
    copy_constructor_test(*mask1, *mask1);
    
    IPAddress mask_copied = *mask1;
    CPPUNIT_ASSERT_EQUAL(mask1->touInt(), mask_copied.touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp(mask1->toStr(), mask_copied.toStr()));
    
    delete mask1;
    
    //leak check
    try{
        mask1 = new IPNetmask((char *)"255.255.1.0");
    }
    catch(sfwdr::Exception::InvalidIPNetmask &e){}
    
}

void FIXTURE_NAME::test_ipnetmask_validation(){
    CPPUNIT_ASSERT_THROW(IPNetmask((char *)"a"), sfwdr::Exception::InvalidIPNetmask);
    CPPUNIT_ASSERT_THROW(IPNetmask((char *)"255.255.1.0"), sfwdr::Exception::InvalidIPNetmask);
    CPPUNIT_ASSERT_THROW(IPNetmask(0xFFFF01FF), sfwdr::Exception::InvalidIPNetmask);
    
    IPNetmask mask2;
    CPPUNIT_ASSERT_THROW(mask2.set((char *)"a"), sfwdr::Exception::InvalidIPNetmask);
    CPPUNIT_ASSERT_THROW(mask2.set((char *)"255.255.1.0"), sfwdr::Exception::InvalidIPNetmask);
    CPPUNIT_ASSERT_THROW(mask2.set(0xFFFF01FF), sfwdr::Exception::InvalidIPNetmask);
    CPPUNIT_ASSERT_THROW(mask2.setLength(33), sfwdr::Exception::InvalidIPNetmask);
    CPPUNIT_ASSERT_THROW(mask2.setLength(-1), sfwdr::Exception::InvalidIPNetmask);
    
    CPPUNIT_ASSERT_EQUAL(0, mask2.getLength());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, mask2.touInt());
}

void FIXTURE_NAME::test_ipnetwork_core(){
    IPNetwork *net;
    
    //[test] not set
    net = new IPNetwork();
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, net->getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0/0", net->toStr()));
    delete net;
    
    //[test] 192.168.0.0/24
    net = new IPNetwork((char *)"192.168.0.0/24");
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"192.168.0.0").touInt(), net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"255.255.255.0").touInt(), net->getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("192.168.0.0/24", net->toStr()));
    delete net;
    
    IPAddress net_addr((char *)"192.168.1.0");
    net = new IPNetwork(net_addr, 24);
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"192.168.1.0").touInt(), net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"255.255.255.0").touInt(), net->getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("192.168.1.0/24", net->toStr()));
    delete net;
    
    //[test] 0.0.0.0/0
    net = new IPNetwork((char *)"0.0.0.0/0");
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"0.0.0.0").touInt(), net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"0.0.0.0").touInt(), net->getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0/0", net->toStr()));
    delete net;
    
    net_addr.set((char *)"0.0.0.0");
    net = new IPNetwork(net_addr, 0);
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"0.0.0.0").touInt(), net->getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"0.0.0.0").touInt(), net->getNetmask().touInt());
    CPPUNIT_ASSERT_EQUAL(0, strcmp("0.0.0.0/0", net->toStr()));
    delete net;
    
    //[test] copy constructor
    net = new IPNetwork((char *)"192.168.0.0/23");
    copy_constructor_test(*net, *net);
    IPNetwork net_copied = *net;
    CPPUNIT_ASSERT_EQUAL(net->getNetaddr().touInt(), net_copied.getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(net->getNetmask().touInt(), net_copied.getNetmask().touInt());
    delete net;
    
    //[test] leak check
    try{
        net = new IPNetwork((char *)"255.255.1.0/23");
    }
    catch(sfwdr::Exception::InvalidIPNetwork &e){}
    
}

void FIXTURE_NAME::test_ipnetwork_validation(){
    CPPUNIT_ASSERT_THROW(IPNetwork((char *)"192.168.1.0/23"), sfwdr::Exception::InvalidIPNetwork);
    CPPUNIT_ASSERT_THROW(IPNetwork((char *)"192.168.0.0"), sfwdr::Exception::InvalidIPNetwork);
    CPPUNIT_ASSERT_THROW(IPNetwork((char *)"s"), sfwdr::Exception::InvalidIPNetwork);
    CPPUNIT_ASSERT_THROW(IPNetwork((char *)"192.168.0.0/24s"), sfwdr::Exception::InvalidIPNetwork);
    
    CPPUNIT_ASSERT_THROW(IPNetwork((char *)"192.168.1.0", 23), sfwdr::Exception::InvalidIPNetwork);
    CPPUNIT_ASSERT_THROW(IPNetwork((char *)"192.168.0.0", 33), sfwdr::Exception::InvalidIPNetwork);
    CPPUNIT_ASSERT_THROW(IPNetwork((char *)"192.168.0.0", -1), sfwdr::Exception::InvalidIPNetwork);
    
    IPNetwork nw;
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"0.0.0.0").touInt(), nw.getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"0.0.0.0").touInt(), nw.getNetmask().touInt());
    CPPUNIT_ASSERT_THROW(nw.set((char *)"192.168.1.0/23"), sfwdr::Exception::InvalidIPNetwork);
    CPPUNIT_ASSERT_EQUAL(IPAddress((char *)"0.0.0.0").touInt(), nw.getNetaddr().touInt());
    CPPUNIT_ASSERT_EQUAL(IPNetmask((char *)"0.0.0.0").touInt(), nw.getNetmask().touInt());
}
