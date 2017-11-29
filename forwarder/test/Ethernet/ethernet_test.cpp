#include<iostream>
#include<cinttypes>
#include "../../src/Ethernet.hpp"
#include "../../src/comlib.hpp"
#include "../../src/dlib.hpp"

int main(void){
    uint8_t packet1[] = {
        0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x08, 0x00,
        0x45, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x40, 0x00, 0x40, 0x06, 0xe2, 0x94, 0xac, 0x11,
        0x00, 0x02, 0xac, 0x11, 0x00, 0x03, 0x13, 0x89, 0xeb, 0x46, 0x92, 0x4d, 0xfc, 0x37,
        0x95, 0x26, 0x28, 0xba, 0xa0, 0x12, 0x71, 0x20, 0xa4, 0xe8, 0x00, 0x00, 0x02, 0x04,
        0x05, 0xb4, 0x04, 0x02, 0x08, 0x0a, 0xad, 0x02, 0x8d, 0x2e, 0x21, 0xed, 0x32, 0x6b,
        0x01, 0x03, 0x03, 0x07
    };
    
    uint8_t packet2[] = {
        0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x08, 0x00,
        0x45, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x40, 0x00, 0x40, 0x06, 0xe2, 0x94, 0xac, 0x11,
        0x00, 0x02, 0xac, 0x11, 0x00, 0x03, 0x13, 0x89, 0xeb, 0x46, 0x92, 0x4d, 0xfc, 0x37,
        0x95, 0x26, 0x28, 0xba, 0xa0, 0x12, 0x71, 0x20, 0xa4, 0xe8, 0x00, 0x00, 0x02, 0x04,
        0x05, 0xb4, 0x04, 0x02, 0x08, 0x0a, 0xad, 0x02, 0x8d, 0x2e, 0x21, 0xed, 0x32, 0x6b,
        0x01, 0x03, 0x03, 0x07
    };
    
    //VLAN 5
    uint8_t packet3[] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x81, 0x00, 0x00, 0x05,
        0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x34, 0x95, 0xdb, 0x2d, 0xb3, 0xf2,
        0xac, 0x12, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xac, 0x12, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    
    //VLAN 3100(0c1c)
    uint8_t packet4[] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x81, 0x00, 0x0c, 0x1c,
        0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x34, 0x95, 0xdb, 0x2d, 0xb3, 0xf2,
        0xac, 0x12, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xac, 0x12, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    Ethernet *eth = new Ethernet();
    eth->set(packet3, sizeof(packet3));
    
    dlib::hexdump(eth->RawData(), eth->getLength());
    
    printf("[eth]type : %.4x\n", eth->getType());
    
    printf("[src]%012" PRIx64 "\n", eth->getSrc().toLong());
    printf("[dst]%012" PRIx64 "\n", eth->getDst().toLong());
    
    printf("[vlan]%u\n", eth->getVlanId());
    
    std::cout << "remove VLAN tag" << std::endl;
    eth->removeVlanTag();
    dlib::hexdump(eth->RawData(), eth->getLength());
    printf("[eth]type : %.4x\n", eth->getType());
    
    std::cout << "set VLAN tag(10)" << std::endl;
    eth->setVlanTag(10);
    dlib::hexdump(eth->RawData(), eth->getLength());
    printf("[eth]type : %.4x\n", eth->getType());
    
    printf("[setter test]");
    Ethernet *eth2 = new Ethernet();
    printf("\n");
    dlib::hexdump(eth2->RawData(), eth2->getLength());
    eth2->setDst(MacAddress("33:44:55:66:77:88"));
    eth2->setSrc(10);
    dlib::hexdump(eth2->RawData(), eth2->getLength());
    
    
    delete eth;
    
    return(0);
}
