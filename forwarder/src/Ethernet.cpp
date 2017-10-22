#include "Ethernet.hpp"

Ethernet::Ethernet(uint8_t *data, int length){
    this->length = length;
    comlib::strncpy((char *)this->data, (char *)data, length);
    this->eth = (struct ETHER *)this->data;
    printf("%.02x %.02x\n", this->eth->eth_type[0], this->eth->eth_type[1]);
    //std::cout << this->eth->eth_type << std::endl;
}
