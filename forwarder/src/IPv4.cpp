#include "IPv4.hpp"

#include <cstdio>

void IPv4::set(uint8_t *eth_ip) {
    this->eth_ip = (ETHER_IPV4 *)eth_ip;
    if (eth_ip != nullptr) _validate();
}

void IPv4::_validate() {
    if (eth_ip == nullptr) return;
    if (eth_ip->version != IPV4_VERSION) {
        throw sfwdr::Exception::Exception((char *)"Invalid IPv4 Packet[version(in header)]");
    }
}

IPAddress IPv4::getSrc() {
    return IPAddress(comlib::ntohl(eth_ip->src));
}

IPAddress IPv4::getDst() {
    return IPAddress(comlib::ntohl(eth_ip->dst));
}

IP_PROTOCOL IPv4::getProtocol() {
    return (IP_PROTOCOL)eth_ip->protocol;
}

void IPv4::setProtocol(IP_PROTOCOL proto) {
    eth_ip->protocol = (uint8_t)proto;
}

void IPv4::setProtocol(uint8_t proto) {
    eth_ip->protocol = proto;
}

uint16_t IPv4::getLength() {
    return (comlib::ntohs(eth_ip->len));
}

void IPv4::setLength(uint32_t length) {
    eth_ip->len = comlib::ntohs(length);
}

uint8_t *IPv4::getPayloadPtr() {
    return &eth_ip->payload;
}
uint16_t IPv4::getHeaderChecksum() {
    return comlib::ntohs(eth_ip->h_checksum);
}

uint16_t IPv4::calcHeaderChecksum(bool set) {
    uint8_t *p;
    uint32_t sum = 0;

    p = (uint8_t *)eth_ip;
    for (int i = 0; i < eth_ip->hlen * 4; i += 2) {
        if (i == 10) {
            continue;
        }

        uint16_t tmp = (uint16_t)(p[i] << 8);
        tmp += p[i + 1];
        sum += tmp;

        if (sum >> 16 == 1) {
            sum = sum - 0x10000 + 0x01;
        }
    }

    if (set) {
        eth_ip->h_checksum = comlib::ntohs((~sum) & 0xFFFF);
        //printf("0x02%x\n", (~sum) & 0xFFFF);
        //eth_ip->h_checksum = 0;
    }

    return ((~sum) & 0xFFFF);
}