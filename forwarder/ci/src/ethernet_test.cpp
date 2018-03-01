#include <cppunit/extensions/HelperMacros.h>
#include <cstring>
#include "Ethernet.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME EthernetTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_addr);
    CPPUNIT_TEST(test_type);
    CPPUNIT_TEST(test_l3_proto);
    CPPUNIT_TEST_SUITE_END();
    
public:
    void setUp();
    void tearDown();
protected:
    void test_addr();
    void test_type();
    void test_l3_proto(); //L3 protocolへの渡し
    void test_vlan(); //VLAN tag 付け外し

private:
    uint8_t arp_packet[64] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x08, 0x06, 0x00, 0x01,
        0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xac, 0x12, 0x00, 0xc8,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xac, 0x12, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    //VLAN ID = 300
    uint8_t arp_packet_vlan[64] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x81, 0x00, 0x01, 0x2c,
        0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
        0xac, 0x12, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xac, 0x12, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    //IP + ICMP
    uint8_t ip_packet[114] = {
        0x11 ,0x11 ,0x11 ,0x11 ,0x11 ,0x11 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x22 ,0x08 ,0x00 ,0x45 ,0x00,
        0x00 ,0x64 ,0x00 ,0x11 ,0x00 ,0x00 ,0xfe ,0x01 ,0x4d ,0x5b ,0xc0 ,0xa8 ,0x01 ,0x64 ,0xac ,0x1f,
        0x01 ,0x01 ,0x08 ,0x00 ,0xd3 ,0xfd ,0x00 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x48,
        0xaa ,0x00 ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd,
        0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd,
        0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd,
        0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd ,0xab ,0xcd,
        0xab ,0xcd
    };
    
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp(){}
void FIXTURE_NAME::tearDown(){}

void FIXTURE_NAME::test_addr(){
    Ethernet *eth = new Ethernet();
    Ethernet *eth_v = new Ethernet();
    eth->set(arp_packet, sizeof(arp_packet));
    eth_v->set(arp_packet_vlan, sizeof(arp_packet_vlan));
    
    //[test]get
    CPPUNIT_ASSERT(MacAddress((char *)"11:11:11:11:11:11") == eth->getSrc());
    CPPUNIT_ASSERT(MacAddress((char *)"FF:FF:FF:FF:FF:FF") == eth->getDst());
    CPPUNIT_ASSERT(MacAddress((char *)"11:11:11:11:11:11") == eth_v->getSrc());
    CPPUNIT_ASSERT(MacAddress((char *)"FF:FF:FF:FF:FF:FF") == eth_v->getDst());
    
    //[test]set
    eth->setSrc(MacAddress((char *)"22:22:22:22:22:22"));
    eth->setDst(MacAddress((char *)"EE:EE:EE:EE:EE:EE"));
    eth_v->setSrc(MacAddress((char *)"22:22:22:22:22:22"));
    eth_v->setDst(MacAddress((char *)"EE:EE:EE:EE:EE:EE"));
    
    CPPUNIT_ASSERT(MacAddress((char *)"22:22:22:22:22:22") == eth->getSrc());
    CPPUNIT_ASSERT(MacAddress((char *)"EE:EE:EE:EE:EE:EE") == eth->getDst());
    CPPUNIT_ASSERT(MacAddress((char *)"22:22:22:22:22:22") == eth_v->getSrc());
    CPPUNIT_ASSERT(MacAddress((char *)"EE:EE:EE:EE:EE:EE") == eth_v->getDst());
    
    eth->setSrc(0x333333333333);
    eth->setDst(0xDDDDDDDDDDDD);
    eth_v->setSrc(0x333333333333);
    eth_v->setDst(0xDDDDDDDDDDDD);
    
    CPPUNIT_ASSERT(MacAddress((char *)"33:33:33:33:33:33") == eth->getSrc());
    CPPUNIT_ASSERT(MacAddress((char *)"DD:DD:DD:DD:DD:DD") == eth->getDst());
    CPPUNIT_ASSERT(MacAddress((char *)"33:33:33:33:33:33") == eth_v->getSrc());
    CPPUNIT_ASSERT(MacAddress((char *)"DD:DD:DD:DD:DD:DD") == eth_v->getDst());
    
    //setで正しくフレームが編集されたかをテストする必要あり
    
    
    delete eth;
    delete eth_v;
}

void FIXTURE_NAME::test_type(){
    Ethernet *eth_arp = new Ethernet();
    Ethernet *eth_arp_v = new Ethernet();
    Ethernet *eth_ip = new Ethernet();
    
    eth_arp->set(arp_packet, sizeof(arp_packet));
    eth_arp_v->set(arp_packet_vlan, sizeof(arp_packet_vlan));
    eth_ip->set(ip_packet, sizeof(ip_packet));
    
    //[test] type
    CPPUNIT_ASSERT(ETHTYPE_ARP == eth_arp->getType());
    CPPUNIT_ASSERT(ETHTYPE_ARP == eth_arp->getULType());
    
    CPPUNIT_ASSERT(ETHTYPE_DOT1Q == eth_arp_v->getType());
    CPPUNIT_ASSERT(ETHTYPE_ARP == eth_arp_v->getULType());
    
    CPPUNIT_ASSERT(ETHTYPE_IPV4 == eth_ip->getType());
    
    delete eth_arp;
    delete eth_arp_v;
    delete eth_ip;
}

void FIXTURE_NAME::test_l3_proto(){
    Ethernet *eth_arp = new Ethernet();
    Ethernet *eth_arp_v = new Ethernet();
    Ethernet *eth_ip = new Ethernet();
    
    eth_arp->set(arp_packet, sizeof(arp_packet));
    eth_arp_v->set(arp_packet_vlan, sizeof(arp_packet_vlan));
    eth_ip->set(ip_packet, sizeof(ip_packet));
    
    //[test] check ARP
    CPPUNIT_ASSERT(true == eth_arp->isARP());
    CPPUNIT_ASSERT(true == eth_arp_v->isARP());
    
    CPPUNIT_ASSERT_NO_THROW(eth_arp->arp());
    CPPUNIT_ASSERT_NO_THROW(eth_arp_v->arp());
    
    CPPUNIT_ASSERT(false == eth_ip->isARP());
    CPPUNIT_ASSERT_THROW(eth_ip->arp(), sfwdr::Exception::InvalidEthType);
    
    //IPv4も要テスト追加
    
    delete eth_arp;
    delete eth_arp_v;
    delete eth_ip;
}

void FIXTURE_NAME::test_vlan(){
    Ethernet *eth = new Ethernet();
    eth->set(arp_packet, sizeof(arp_packet));
    
    //[test] vlan remove and set
    CPPUNIT_ASSERT_EQUAL(64, (int)eth->getLength());
    CPPUNIT_ASSERT_EQUAL(false, eth->hasVlan());
    CPPUNIT_ASSERT_EQUAL(0, (int)eth->getVlanId());
    
    CPPUNIT_ASSERT_EQUAL(64, (int)eth->removeVlanTag());
    CPPUNIT_ASSERT_EQUAL(64, (int)eth->getLength());
    CPPUNIT_ASSERT_EQUAL(false, eth->hasVlan());
    CPPUNIT_ASSERT_EQUAL(0, memcmp(arp_packet, eth->RawData(), 64));
    
    CPPUNIT_ASSERT_EQUAL(68, (int)eth->setVlanTag(300));
    CPPUNIT_ASSERT_EQUAL(68, (int)eth->getLength());
    CPPUNIT_ASSERT_EQUAL(true, eth->hasVlan());
    CPPUNIT_ASSERT_EQUAL(300, (int)eth->getVlanId());
    CPPUNIT_ASSERT_EQUAL(0, memcmp(arp_packet_vlan, eth->RawData(), 64));
    
    CPPUNIT_ASSERT_EQUAL(68, (int)eth->setVlanTag(300));
    CPPUNIT_ASSERT_EQUAL(68, (int)eth->getLength());
    CPPUNIT_ASSERT_EQUAL(true, eth->hasVlan());
    CPPUNIT_ASSERT_EQUAL(0, memcmp(arp_packet_vlan, eth->RawData(), 64));
    
    CPPUNIT_ASSERT_EQUAL(64, (int)eth->removeVlanTag());
    CPPUNIT_ASSERT_EQUAL(64, (int)eth->getLength());
    CPPUNIT_ASSERT_EQUAL(false, eth->hasVlan());
    CPPUNIT_ASSERT_EQUAL(0, memcmp(arp_packet, eth->RawData(), 64));
    
    delete eth;
}

