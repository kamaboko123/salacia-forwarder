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

struct TCP{
    uint16_t port_src;
    uint16_t port_dst;
    uint32_t seq;
    uint32_t ack_seq;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    //2byte(16bit)
    //ネットワークはビッグエンディアン、CPUはリトルエンディアン
    
    //TCPヘッダはデータオフセット[4bit], 予約[6bit], flag[6bit]で並ぶ
    //前の1byte(データオフセット4bit + 予約2bit)
    //これでd_offset(4), reserve1(4)で並ぶ
    uint16_t reserve1:4;
    uint16_t d_offset:4;
    
    //後ろの1byte(flag 6bit + 予約2bit)
    //これでreserve2(2), urg, ack, psh, rst, syn, finで並ぶ
    uint16_t fin:1;
    uint16_t syn:1;
    uint16_t rst:1;
    uint16_t psh:1;
    uint16_t ack:1;
    uint16_t urg:1;
    uint16_t reserve2:2;
    
#elif __BYTE_ORDER == __BIG_ENDIAN
    //CPUがビッグエンディアンの場合はそのまま
    uint16_t d_offset:4;
    uint16_t reserve1:4;
    uint16_t reserve2:2;
    uint16_t urg:1;
    uint16_t ack:1;
    uint16_t psh:1;
    uint16_t rst:1;
    uint16_t syn:1;
    uint16_t fin:1;
#else
# error "Please fix <bits/endian.h>"
#endif
    uint16_t winsize;
    uint16_t checksum;
    uint16_t urg_p;
    
    uint32_t *options;
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
    uint16_t packet_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t ip_src;
    uint32_t ip_dst;
    
    union {
        struct TCP tcp;
    }payload;
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
