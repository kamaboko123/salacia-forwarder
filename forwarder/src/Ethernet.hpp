#ifndef INCLUDED_ETHERNET
#define INCLUDED_ETHERNET

#include <iostream>
#include <cstdint>
#include <cstdio>
#include "MacAddress.hpp"
#include "comlib.hpp"

enum EthType{
    ETHTYPE_ARP = 0x0806,
    ETHTYPE_IPV4 = 0x0800,
    ETHTYPE_UNKNOWN = 0xFFFF //RFC的にはreservd
};

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
    
    uint64_t mactol(uint8_t *mac_addr);
    
public:
    Ethernet();
    Ethernet(uint8_t *data, int length);
    void set(uint8_t *data, int length);
    
    EthType getType();
    MacAddress getDst();
    MacAddress getSrc();
};

#endif
