#include "Ethernet.hpp"

Ethernet::Ethernet() {
    this->eth = (struct ETHER *)this->data;
    return;
}

Ethernet::Ethernet(const uint8_t *eth_data, uint16_t len) {
    set(eth_data, len);
}

void Ethernet::set(const uint8_t *eth_data, uint16_t len) {
    this->length = len;
    comlib::memcpy(this->data, eth_data, length);
    this->eth = (struct ETHER *)this->data;

    _update();
}

void Ethernet::setLength(const uint16_t len) {
    this->length = len;
}

void Ethernet::_update() {
    _type = (EthType)comlib::ntohs(eth->eth_type);
    _ul_head = ((uint8_t *)&(eth->eth_type)) + sizeof(eth->eth_type);
    _ul_type = _type;

    //_type = type;
    switch (_type) {
        case ETHTYPE_ARP:
            _arp.set(_ul_head);
            break;
        case ETHTYPE_IPV4:
            _ipv4.set(_ul_head);
            break;
        case ETHTYPE_DOT1Q: {
            struct DOT1Q *tag = (struct DOT1Q *)_ul_head;
            _ul_type = (EthType)comlib::ntohs(tag->eth_type);
            _ul_head += DOT1Q_TAG_SIZE;
            break;
        }
        default:
            _type = ETHTYPE_UNKNOWN;
    }

    switch (_ul_type) {
        case ETHTYPE_ARP:
            _arp.set(_ul_head);
            break;
        case ETHTYPE_IPV4:
            break;
        default:
            _ul_type = ETHTYPE_UNKNOWN;
    }
}

EthType Ethernet::getType() const {
    return (_type);
}

EthType Ethernet::getULType() const {
    return (_ul_type);
}

uint16_t Ethernet::getLength() const {
    return (length);
}

void Ethernet::setType(EthType type) {
    eth->eth_type = comlib::htons(type);
}

MacAddress Ethernet::getDst() const {
    return (MacAddress(comlib::bytestol(eth->dst_mac, 6)));
}

MacAddress Ethernet::getSrc() const {
    return (MacAddress(comlib::bytestol(eth->src_mac, 6)));
}

void Ethernet::setDst(const MacAddress &addr) {
    setDst(addr.toLong());
}
void Ethernet::setSrc(const MacAddress &addr) {
    setSrc(addr.toLong());
}

void Ethernet::setDst(uint64_t addr) {
    uint8_t *p = (uint8_t *)&addr;
    for (int i = 0; i < 6; i++) {
        eth->dst_mac[5 - i] = p[i];
    }
}

void Ethernet::setSrc(uint64_t addr) {
    uint8_t *p = (uint8_t *)&addr;
    for (int i = 0; i < 6; i++) {
        eth->src_mac[5 - i] = p[i];
    }
}

uint16_t Ethernet::getVlanId() const {
    uint16_t id = 0;

    struct DOT1Q *tag = (struct DOT1Q *)&this->data[ETH_H_SIZE];
    if (getType() == ETHTYPE_DOT1Q) {
        //ホストのバイトオーダにして12bit取り出し
        uint16_t tmp = comlib::ntohs(tag->tci);
        id = tmp & 0x0FFF;
    }

    return (id);
}

uint16_t Ethernet::removeVlanTag() {
    if (getType() != ETHTYPE_DOT1Q) return (length);

    //VLANの中身を前に上書く
    comlib::memmove(&data[MAC_ADDR_SIZE * 2], &data[(MAC_ADDR_SIZE * 2) + DOT1Q_TAG_SIZE], length - DOT1Q_TAG_SIZE);

    length -= DOT1Q_TAG_SIZE;

    _update();
    return (length);
}

uint16_t Ethernet::setVlanTag(uint16_t vlan_id) {
    if (getType() == ETHTYPE_DOT1Q) return (length);

    //tagの分ずらす
    comlib::memmove(&data[(MAC_ADDR_SIZE * 2) + DOT1Q_TAG_SIZE], &data[(MAC_ADDR_SIZE * 2)], length + DOT1Q_TAG_SIZE);

    //tagセット
    data[(MAC_ADDR_SIZE * 2)] = ETHTYPE_DOT1Q >> 8;
    data[(MAC_ADDR_SIZE * 2) + 1] = (uint8_t)(ETHTYPE_DOT1Q & 0x0FF);

    DOT1Q *tag = (struct DOT1Q *)&data[ETH_H_SIZE];
    //ネットワークバイトオーダにしてセット(CoSとかは現状未対応)
    tag->tci = comlib::htons(vlan_id);
    length += DOT1Q_TAG_SIZE;

    _update();
    return (length);
}

uint8_t *Ethernet::RawData() {
    return (data);
}

bool Ethernet::hasVlan() const {
    return (getULType() == ETHTYPE_DOT1Q);
}

bool Ethernet::isARP() const {
    return (getULType() == ETHTYPE_ARP);
}

ARP &Ethernet::arp() {
    if (!isARP()) throw sfwdr::Exception::InvalidEthType("ARP");
    return (_arp);
}

bool Ethernet::isIPv4() const {
    return (getULType() == ETHTYPE_IPV4);
}

IPv4 &Ethernet::ipv4() {
    if (!isIPv4()) throw sfwdr::Exception::InvalidEthType("IPv4");
    return (_ipv4);
}
