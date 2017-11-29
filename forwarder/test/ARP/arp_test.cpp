#include<iostream>
#include<cinttypes>
#include "../../src/Ethernet.hpp"
#include "../../src/comlib.hpp"
#include "../../src/dlib.hpp"

int main(void){
    uint8_t arp_base[] = PKT_ARP;
    Ethernet eth1(arp_base, PKT_ARP_SIZE);
    
    printf("[type] : %.4x\n", eth1.getType());
    printf("[src]%012" PRIx64 "\n", eth1.getSrc().toLong());
    printf("[dst]%012" PRIx64 "\n", eth1.getDst().toLong());
    
    eth1.arp();
    
    return(0);
}
