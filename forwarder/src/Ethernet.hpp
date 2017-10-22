#ifndef INCLUDED_ETHERNET
#define INCLUDED_ETHERNET

#include <iostream>
#include <cstdint>
#include <cstdio>
#include "comlib.hpp"

struct ETHER{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint8_t eth_type[2];
    
    /*
    union {
        struct ARP arp;
        struct IP ip;
    }payload;
    */
    //uint8_t padding[18];
} __attribute__((__packed__));


class Ethernet{
private:
    uint8_t data[1500];
    struct ETHER *eth;
    int length;
public:
    Ethernet(uint8_t *data, int length);
};

#endif
