#ifndef INCLUDED_ARP
#define INCLUDED_ARP

#include <stdio.h>
#include "MacAddress.hpp"
#include "IPAddress.hpp"
#include "Exception.hpp"
#include "comlib.hpp"

#define PKT_ARP \
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x08, 0x06, 0x00, 0x01,\
0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0xAC, 0x12, 0x00, 0xC8,\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAC, 0x12, 0x00, 0x01}

#define PKT_ARP_SIZE 42

#define ARP_HW_TYPE_ETHER 1
#define ARP_PROTO_TYPE_IPV4 0x0800

struct ETHER_ARP{
    uint16_t hw_type;
    uint16_t proto_type;
    uint8_t hlen;
    uint8_t plen;
    uint16_t op_code;
    uint8_t src_mac[6];
    uint32_t src_ip;
    uint8_t dst_mac[6];
    uint32_t dst_ip;
} __attribute__((__packed__));

enum ARPOPCode : uint16_t{
    ARP_REQUEST = 0x0001,
    ARP_REPLY = 0x0002,
    RARP_REQUEST = 0x0003,
    RARP_REPLY = 0x0004,
};

class ARP{
private:
    ETHER_ARP *eth_arp;
    
    void _nullexc() const;
    void _validate();
public:
    ARP();
    ARP(uint8_t *arp_head);
    
    void set(uint8_t *arp_head);
    
    void setOPCode(ARPOPCode code);
    ARPOPCode getOPCode() const;
    
    void setSrcMac(const MacAddress &src_mac);
    void setSrcMac(const uint64_t &src_mac);
    MacAddress getSrcMac() const;
    
    void setDstMac(const MacAddress &dst_mac);
    void setDstMac(const uint64_t &dst_mac);
    MacAddress getDstMac() const;
    
    void setSrcIP(const IPAddress &src_ip);
    void setSrcIP(const uint32_t &src_ip);
    IPAddress getSrcIP() const;
    
    void setDstIP(const IPAddress &dst_ip);
    void setDstIP(const uint32_t &dst_ip);
    IPAddress getDstIP() const;
};

#endif
