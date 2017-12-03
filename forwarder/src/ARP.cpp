#include "ARP.hpp"

ARP::ARP(){
    set(nullptr);
}

void ARP::_nullexc() const{
    if(eth_arp == nullptr) throw Exception::Exception((char *)"ARP data is not set");
}

ARP::ARP(uint8_t *arp_head){
    set(arp_head);
}

void ARP::set(uint8_t *arp_head){
    //head = arp_head;
    eth_arp = (ETHER_ARP *)arp_head;
    if(eth_arp != nullptr) _validate();
}

void ARP::_validate(){
    if(eth_arp == nullptr) return;
    if(comlib::ntohs(eth_arp->hw_type) != ARP_HW_TYPE_ETHER){
        throw Exception::Exception((char *)"Invalid ARP Packet[hw_ype]");
    }
    if(comlib::ntohs(eth_arp->proto_type) != ARP_PROTO_TYPE_IPV4){
        throw  Exception::Exception((char *)"Invalid ARP Packet[proto_type]");
    }
    
    if(eth_arp->hlen != MAC_ADDR_SIZE){
        throw  Exception::Exception((char *)"Invalid ARP Packet[hlen]");
    }
    if(eth_arp->plen != IP_ADDR_SIZE){
        throw  Exception::Exception((char *)"Invalid ARP Packet[plen]");
    }
}

void ARP::setOPCode(ARPOPCode code){
    _nullexc();
    eth_arp->op_code = comlib::htons((uint16_t)code);
}

void ARP::setSrcMac(const MacAddress &src_mac){
    setSrcMac(src_mac.toLong());
}

void ARP::setSrcMac(const uint64_t &src_mac){
    uint8_t *p = (uint8_t *)&src_mac;
    for(int i = 0; i < MAC_ADDR_SIZE; i++){
        eth_arp->src_mac[MAC_ADDR_SIZE - (i + 1)] = p[i];
    }
}

void ARP::setDstMac(const MacAddress &dst_mac){
    setDstMac(dst_mac.toLong());
}

void ARP::setDstMac(const uint64_t &dst_mac){
    uint8_t *p = (uint8_t *)&dst_mac;
    for(int i = 0; i < MAC_ADDR_SIZE; i++){
        eth_arp->dst_mac[MAC_ADDR_SIZE - (i + 1)] = p[i];
    }
}

void ARP::setSrcIP(const IPAddress &src_ip){
    setSrcIP(src_ip.touInt());
}

void ARP::setSrcIP(const uint32_t &src_ip){
    eth_arp->src_ip = comlib::htonl(src_ip);
}

void ARP::setDstIP(const IPAddress &dst_ip){
    setDstIP(dst_ip.touInt());
}

void ARP::setDstIP(const uint32_t &dst_ip){
    eth_arp->dst_ip = comlib::htonl(dst_ip);
}




ARPOPCode ARP::getOPCode() const{
    _nullexc();
    return((ARPOPCode)comlib::ntohs(eth_arp->op_code));
}

MacAddress ARP::getSrcMac() const{
    _nullexc();
    return(MacAddress(comlib::bytestol((uint8_t *)&eth_arp->src_mac, MAC_ADDR_SIZE)));
}

MacAddress ARP::getDstMac() const{
    _nullexc();
    return(MacAddress(comlib::bytestol((uint8_t *)&eth_arp->dst_mac, MAC_ADDR_SIZE)));
}

IPAddress ARP::getDstIP() const{
    _nullexc();
    return(IPAddress(comlib::bytestol((uint8_t *)&eth_arp->dst_ip, IP_ADDR_SIZE)));
}

IPAddress ARP::getSrcIP() const{
    _nullexc();
    return(IPAddress(comlib::bytestol((uint8_t *)&eth_arp->src_ip, IP_ADDR_SIZE)));
}

