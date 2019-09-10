#include <cstring>
#include <cppunit/extensions/HelperMacros.h>
#include "Ethernet.hpp"
#include "IPv4.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME IPv4Test

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
    Ethernet pkt1(PKT_IPV4, sizeof(PKT_IPV4));
    CPPUNIT_ASSERT(IPAddress("172.16.0.101") == pkt1.ipv4().getSrc());
    CPPUNIT_ASSERT(IPAddress("8.8.8.8") == pkt1.ipv4().getDst());
    CPPUNIT_ASSERT(PROTOCOL_ICMP == pkt1.ipv4().getProtocol());
    CPPUNIT_ASSERT(84 == pkt1.ipv4().getLength());
}

