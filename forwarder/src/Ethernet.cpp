#include "Ethernet.hpp"

Ethernet::Ethernet(){
    return;
}

Ethernet::Ethernet(uint8_t *eth_data, int len){
    set(eth_data, len);
}

void Ethernet::set(uint8_t *eth_data, int len){
    this->length = len;
    comlib::memcpy(this->data, eth_data, length);
    this->eth = (struct ETHER *)this->data;
    /*
    if(getType() == ETHTYPE_DOT1Q){
        this->eth_dot1q = (struct ETHER_DOT1Q *)this->data;
    }*/
    //printf("%.02x %.02x\n", this->eth->eth_type[0], this->eth->eth_type[1]);
    //std::cout << this->eth->eth_type << std::endl;
}

EthType Ethernet::getType(){
    uint16_t type = 0;
    type = (uint16_t)eth->eth_type[0] << 8;
    type += (uint16_t)eth->eth_type[1];
    
    EthType ret;
    switch(type){
        case ETHTYPE_ARP:
            ret =  ETHTYPE_ARP;
            break;
        case ETHTYPE_IPV4:
            ret = ETHTYPE_IPV4;
            break;
        case ETHTYPE_DOT1Q:
            ret = ETHTYPE_DOT1Q;
            break;
        default :
            ret = ETHTYPE_UNKNOWN;
    }
    
    return(ret);
}

MacAddress Ethernet::getDst(){
    return(MacAddress(mactol(eth->dst_mac)));
}

MacAddress Ethernet::getSrc(){
    return(MacAddress(mactol(eth->src_mac)));
}

uint64_t Ethernet::mactol(uint8_t *mac_addr){
    return(comlib::bytestol(mac_addr, 6));
}

uint16_t Ethernet::getVlanId(){
    uint16_t id = 0;
    if(getType() == ETHTYPE_DOT1Q){
        printf("!%.02x %.02x", eth->payload.dot1q.vlan1, eth->payload.dot1q.vlan2);
        id = (uint16_t)eth->payload.dot1q.vlan2 << 4;
        id += (uint16_t)eth->payload.dot1q.vlan1;
    }
    
    return(id);
}
