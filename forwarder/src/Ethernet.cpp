#include "Ethernet.hpp"

Ethernet::Ethernet(){
    this->eth = (struct ETHER *)this->data;
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
    uint16_t type = comlib::ntohs(eth->eth_type);
    
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
    eth->eth_type = comlib::htons(type);
}

MacAddress Ethernet::getDst(){
    return(MacAddress(mactol(eth->dst_mac)));
}

MacAddress Ethernet::getSrc(){
    return(MacAddress(mactol(eth->src_mac)));
}

void Ethernet::setDst(MacAddress addr){
    setDst(addr.toLong());
}
void Ethernet::setSrc(MacAddress addr){
    setSrc(addr.toLong());
}

void Ethernet::setDst(uint64_t addr){
    uint8_t *p = (uint8_t *)&addr;
    for(int i = 0; i < 6; i++){
        eth->dst_mac[5 - i] = p[i];
    }
}

void Ethernet::setSrc(uint64_t addr){
    uint8_t *p = (uint8_t *)&addr;
    for(int i = 0; i < 6; i++){
        eth->src_mac[5 - i] = p[i];
    }
}

uint64_t Ethernet::mactol(uint8_t *mac_addr){
    return(comlib::bytestol(mac_addr, MAC_ADDR_SIZE));
}

uint16_t Ethernet::getVlanId(){
    uint16_t id = 0;
    struct DOT1Q *tag = (struct DOT1Q *)&this->data[ETH_H_SIZE];
    if(getType() == ETHTYPE_DOT1Q){
        //ホストのバイトオーダにして12bit取り出し
        uint16_t tmp = comlib::ntohs(tag->tci);
        id = tmp & 0x0FFF;
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
    //ネットワークバイトオーダにしてセット(CoSとかは現状未対応)
    tag->tci = comlib::htons(vlan_id);
    length += DOT1Q_TAG_SIZE;
    
    return(length);
}

uint8_t *Ethernet::RawData(){
    return(data);
}
