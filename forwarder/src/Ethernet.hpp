#ifndef INCLUDED_ETHERNET
#define INCLUDED_ETHERNET

#include <iostream>
#include <cstdint>
#include <cstdio>
#include "MacAddress.hpp"
#include "comlib.hpp"
#include "dlib.hpp"
#include "ARP.hpp"

using namespace sfwdr::Exception;

#define ETH_BUF_SIZE 2048
#define ETH_MIN_SIZE 56
#define MAC_ADDR_SIZE 6
#define ETH_H_SIZE 14
#define DOT1Q_TAG_SIZE 4

enum EthType{
    ETHTYPE_ARP = 0x0806,
    ETHTYPE_IPV4 = 0x0800,
    ETHTYPE_DOT1Q = 0x8100,
    ETHTYPE_UNKNOWN = 0xFFFF //RFC的にはreserved
};

struct DOT1Q{
    uint16_t tci;
} __attribute__((__packed__));

struct ETHER{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t eth_type;
    uint8_t _n_head;
} __attribute__((__packed__));


class Ethernet{
private:
    uint8_t data[ETH_BUF_SIZE] = {0};
    struct ETHER *eth;
    struct ETHER_DOT1Q *eth_dot1q;
    uint16_t length = 12;
    ARP _arp;
    
    uint64_t mactol(uint8_t *mac_addr);
    void setType(EthType type);
    
public:
    Ethernet();
    Ethernet(uint8_t *data, uint16_t length);
    void set(uint8_t *data, uint16_t length);
    
    uint16_t getLength();
    void setLength(uint16_t len);
    
    EthType getType();
    
    MacAddress getDst();
    MacAddress getSrc();
    
    void setDst(MacAddress addr);
    void setDst(uint64_t addr);
    void setSrc(MacAddress addr);
    void setSrc(uint64_t addr);
    
    bool isVlan();
    bool isARP();
    
    uint16_t getVlanId();
    uint16_t removeVlanTag();
    uint16_t setVlanTag(uint16_t vlan_id);
    uint8_t *RawData();
    
    ARP &arp();
};

#endif
