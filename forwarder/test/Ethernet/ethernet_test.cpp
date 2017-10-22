#include<iostream>
#include "../../src/Ethernet.hpp"

int main(void){
    uint8_t packet[] = {
        0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x08, 0x00,
        0x45, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x40, 0x00, 0x40, 0x06, 0xe2, 0x94, 0xac, 0x11,
        0x00, 0x02, 0xac, 0x11, 0x00, 0x03, 0x13, 0x89, 0xeb, 0x46, 0x92, 0x4d, 0xfc, 0x37,
        0x95, 0x26, 0x28, 0xba, 0xa0, 0x12, 0x71, 0x20, 0xa4, 0xe8, 0x00, 0x00, 0x02, 0x04,
        0x05, 0xb4, 0x04, 0x02, 0x08, 0x0a, 0xad, 0x02, 0x8d, 0x2e, 0x21, 0xed, 0x32, 0x6b,
        0x01, 0x03, 0x03, 0x07
    };
    
    Ethernet *eth = new Ethernet(packet, sizeof(packet));
    
    return(0);
}
