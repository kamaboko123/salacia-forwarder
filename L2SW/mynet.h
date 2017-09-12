#ifndef MYNET_H
#define MYNET_H

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/if.h>

#define TYPE_ARP 0x0806
#define TYPE_IP4 0x0800

struct ARP{
    uint8_t hw_type[2];
    uint8_t proto_type[2];
    uint8_t hlen[1];
    uint8_t plen[1];
    uint8_t op_code[2];
    uint8_t src_mac[6];
    uint8_t src_ip[4];
    uint8_t dst_mac[6];
    uint8_t dst_ip[4];
    uint8_t padding[18];
} __attribute__((__packed__));

struct IP{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error "Please fix <bits/endian.h>"
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
} __attribute__((__packed__));

struct ETHER{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint8_t eth_type[2];
    
    union {
        struct ARP arp;
        struct IP ip;
    }payload;
    
    //uint8_t padding[18];
} __attribute__((__packed__));

#endif
