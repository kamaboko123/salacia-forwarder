#include "Ethernet.hpp"

Ethernet::Ethernet(){
    return;
}

Ethernet::Ethernet(uint8_t *eth_data, uint16_t len){
    set(eth_data, len);
}

void Ethernet::set(uint8_t *eth_data, uint16_t len){
    this->length = len;
    comlib::memcpy(this->data, eth_data, length);
    this->eth = (struct ETHER *)this->data;
}

EthType Ethernet::getType(){
    //uint16_t type = (uint16_t)eth->eth_type;
    
    uint16_t type;
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

uint16_t Ethernet::getLength(){
    return(length);
}

void Ethernet::setType(EthType type){
    //eth->eth_type = type;
    eth->eth_type[0] = (uint8_t)type >> 8;
    eth->eth_type[1] = (uint8_t)type;
}

MacAddress Ethernet::getDst(){
    return(MacAddress(mactol(eth->dst_mac)));
}

MacAddress Ethernet::getSrc(){
    return(MacAddress(mactol(eth->src_mac)));
}

uint64_t Ethernet::mactol(uint8_t *mac_addr){
    return(comlib::bytestol(mac_addr, MAC_ADDR_SIZE));
}

uint16_t Ethernet::getVlanId(){
    uint16_t id = 0;
    struct DOT1Q *tag = (struct DOT1Q *)&this->data[ETH_H_SIZE];
    if(getType() == ETHTYPE_DOT1Q){
        //printf("!%.02x %.02x\n", tag->vlan1, tag->vlan2);
        id = (uint16_t)tag->vlan1 << 8;
        id += (uint16_t)tag->vlan2;
    }
    
    return(id);
}

uint16_t Ethernet::removeVlanTag(){
    if(getType() != ETHTYPE_DOT1Q) return(length);
    
    //VLANの中身を前に上書く
    comlib::memmove(&data[MAC_ADDR_SIZE * 2], &data[(MAC_ADDR_SIZE * 2) + DOT1Q_TAG_SIZE], length - DOT1Q_TAG_SIZE);
    
    length -= DOT1Q_TAG_SIZE;
    return(length);
}

uint16_t Ethernet::setVlanTag(uint16_t vlan_id){
    if(getType() == ETHTYPE_DOT1Q) return(length);
    
    //tagの分ずらす
    comlib::memmove(&data[(MAC_ADDR_SIZE * 2) + DOT1Q_TAG_SIZE], &data[(MAC_ADDR_SIZE * 2)], length + DOT1Q_TAG_SIZE);
    
    //tagセット
    data[(MAC_ADDR_SIZE * 2)] = ETHTYPE_DOT1Q >> 8;
    data[(MAC_ADDR_SIZE * 2) + 1] = (uint8_t)(ETHTYPE_DOT1Q & 0x0FF);
    
    DOT1Q *tag = (struct DOT1Q *)&data[ETH_H_SIZE];
    tag->vlan1 = (uint16_t) vlan_id >> 8;
    tag->vlan2 = (uint16_t) vlan_id;
    length += DOT1Q_TAG_SIZE;
    
    return(length);
}

uint8_t *Ethernet::RawData(){
    return(data);
}
