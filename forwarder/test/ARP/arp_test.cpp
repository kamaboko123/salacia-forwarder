#include<iostream>
#include<cinttypes>
#include "../../src/Ethernet.hpp"
#include "../../src/comlib.hpp"
#include "../../src/dlib.hpp"

int main(void){
    uint8_t arp_base[] = PKT_ARP;
    Ethernet eth1(arp_base, PKT_ARP_SIZE);
    printf("[type] : %.4x\n", eth1.getULType());
    printf("[src]%012" PRIx64 "\n", eth1.getSrc().toLong());
    printf("[dst]%012" PRIx64 "\n", eth1.getDst().toLong());
    
    try{
        eth1.arp();
    }
    catch(Exception &e){
        printf("[Exception]%s\n", e.getMessage());
    }
    
    return(0);
}
