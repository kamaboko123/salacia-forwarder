#include "ARP.hpp"

ARP::ARP(){
    set(nullptr);
}

void ARP::_nullexc(){
    if(head == nullptr) throw Exception((char *)"ARP data is not set");
}

ARP::ARP(uint8_t *arp_head){
    set(arp_head);
}

void ARP::set(uint8_t *arp_head){
    head = arp_head;
}

