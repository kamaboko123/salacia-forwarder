#ifndef INCLUDED_IPv4
#define INCLUDED_IPv4

#include <cstdint>

#include "Exception.hpp"
#include "IPAddress.hpp"
#include "comlib.hpp"

const uint8_t PKT_IPV4[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x08, 0x00, 0x45, 0x00,
    0x00, 0x54, 0x78, 0xb0, 0x40, 0x00, 0x40, 0x01, 0x05, 0x74, 0xac, 0x10, 0x00, 0x65, 0x08, 0x08,
    0x08, 0x08};

#define IPV4_VERSION 4
enum IP_PROTOCOL {
    PROTOCOL_ICMP = 1,
    PROTOCOL_TCP = 6,
    PROTOCOL_UDP = 17
};

struct ETHER_IPV4 {
    uint8_t hlen : 4;
    uint8_t version : 4;
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
    uint8_t payload;
} __attribute__((__packed__));

class IPv4 {
   private:
    ETHER_IPV4 *eth_ip;
    void _validate();

   public:
    void set(uint8_t *eth_ip);
    IPAddress getSrc();
    IPAddress getDst();
    IP_PROTOCOL getProtocol();
    void setProtocol(IP_PROTOCOL proto);
    void setProtocol(uint8_t proto);
    uint16_t getLength();
    void setLength(uint32_t length);
    uint8_t *getPayloadPtr();
    uint16_t calcHeaderChecksum(bool set = true);
    uint16_t getHeaderChecksum();
};

#endif
