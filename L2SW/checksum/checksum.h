#ifndef CALCADDR_H
#define CALCADDR_H

#include "../mynet.h"

struct PSEUDO_IP{
    uint32_t ip_src;
    uint32_t ip_dst;
    uint8_t dummy;
    uint8_t ip_proto;
    uint16_t ip_len;
}__attribute__((__packed__));

uint16_t checksum_ip(struct IP *ip);
uint16_t checksum_tcp(struct IP *ip);

#endif
