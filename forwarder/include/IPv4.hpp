#ifndef INCLUDED_IPv4
#define INCLUDED_IPv4

#include <cstdint>
#include "Exception.hpp"
#include "comlib.hpp"
#include "IPAddress.hpp"

#define IPV4_VERSION 4
enum IP_PROTOCOL{
    PROTOCOL_ICMP = 1,
    PROTOCOL_TCP = 6,
    PROTOCOL_UDP = 17
};

struct ETHER_IPV4{
    uint8_t hlen: 4;
    uint8_t version: 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t flg_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t h_checksum;
    uint32_t src;
    uint32_t dst;
    uint32_t opt_padding;
} __attribute__((__packed__));

class IPv4{
private:
    ETHER_IPV4 *eth_ip;
    void _validate();
public:
    void set(uint8_t *eth_ip);
    IPAddress getSrc();
    IPAddress getDst();
    IP_PROTOCOL getProtocol();
    uint16_t getLength();
};

#endif
