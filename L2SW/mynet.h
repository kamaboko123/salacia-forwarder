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

struct ETHER{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint8_t eth_type[2];
    
    union {
        struct ARP arp;
    }payload;
    
    //uint8_t padding[18];
} __attribute__((__packed__));

#endif
