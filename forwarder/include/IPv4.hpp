#ifndef INCLUDED_IPv4
#define INCLUDED_IPv4

#include <cstdint>

struct ETHER_IP{
    uint8_t hlen: 4;
    uint8_t version: 4;
} __attribute__((__packed__));

class IPv4{
private:
public:
    void set(uint8_t *eth_ip);
};

#endif
