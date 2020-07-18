#ifndef INCLUDED_ETHERNET
#define INCLUDED_ETHERNET

#include <cstdint>
#include <cstdio>
#include <iostream>

#include "ARP.hpp"
#include "IPv4.hpp"
#include "MacAddress.hpp"
#include "comlib.hpp"
#include "dlib.hpp"

#define ETH_BROADCAST 0xFFFFFFFF

#define ETH_BUF_SIZE 4096
#define ETH_MIN_SIZE 56
#define ETH_H_SIZE 14
#define DOT1Q_TAG_SIZE 4

enum EthType : uint16_t {
    ETHTYPE_ARP = 0x0806,
    ETHTYPE_IPV4 = 0x0800,
    ETHTYPE_DOT1Q = 0x8100,
    ETHTYPE_UNKNOWN = 0xFFFF  //RFC的にはreserved
};

struct DOT1Q {
    uint16_t tci;
    uint16_t eth_type;
} __attribute__((__packed__));

struct ETHER {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t eth_type;
} __attribute__((__packed__));

class Ethernet {
   private:
    uint8_t data[ETH_BUF_SIZE] = {0};
    struct ETHER *eth;
    struct ETHER_DOT1Q *eth_dot1q;
    uint16_t length = 12;

    uint16_t _vlan = 0;  //0はタグなしを示す
    EthType _type;
    EthType _ul_type;

    ARP _arp;
    IPv4 _ipv4;

    //upper layer header
    uint8_t *_ul_head;

    void _update();
    void setType(EthType type);

   public:
    Ethernet();
    Ethernet(const uint8_t *data, uint16_t length);
    void set(const uint8_t *data, uint16_t length);

    uint16_t getLength() const;
    void setLength(const uint16_t len);

    EthType getType() const;
    EthType getULType() const;

    MacAddress getDst() const;
    MacAddress getSrc() const;

    void setDst(const MacAddress &addr);
    void setDst(uint64_t addr);
    void setSrc(const MacAddress &addr);
    void setSrc(uint64_t addr);

    bool hasVlan() const;
    bool isARP() const;
    bool isIPv4() const;

    uint16_t getVlanId() const;
    uint16_t removeVlanTag();
    uint16_t setVlanTag(uint16_t vlan_id);
    uint8_t *RawData();

    ARP &arp();
    IPv4 &ipv4();
};

#endif
