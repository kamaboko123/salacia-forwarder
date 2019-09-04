#include "IPv4.hpp"
#include <cstdio>

void IPv4::set(uint8_t *eth_ip){
    this->eth_ip = (ETHER_IPV4 *)eth_ip;
    if(eth_ip != nullptr) _validate();
}

void IPv4::_validate(){
    if(eth_ip == nullptr) return;
    if(eth_ip->version != IPV4_VERSION){
        throw sfwdr::Exception::Exception((char *)"Invalid IPv4 Packet[version(in header)]");
    }
}

IPAddress IPv4::getSrc(){
    return IPAddress(comlib::ntohl(eth_ip->src));
}

IPAddress IPv4::getDst(){
    return IPAddress(comlib::ntohl(eth_ip->dst));
}

IP_PROTOCOL IPv4::getProtocol(){
    return (IP_PROTOCOL)eth_ip->protocol;
}

uint16_t IPv4::getLength(){
    return(comlib::ntohs(eth_ip->len));
}
