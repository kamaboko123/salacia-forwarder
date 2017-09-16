#include "endian.h"

uint32_t swap_32(uint32_t n){
    uint32_t ret = 0;
    uint8_t *p;
    
    ret += (n & 0x000000FF) << 24;
    ret += (n & 0x0000FF00) << 8;
    ret += (n & 0x00FF0000) >> 8;
    ret += (n & 0xFF000000) >> 24;
    
    return(ret);
}

uint16_t swap_16(uint16_t n){
    uint16_t ret = 0;
    uint8_t *p;
    
    ret += (n & 0x00FF) << 8;
    ret += (n & 0xFF00) >> 8;
    
    return(ret);
}

