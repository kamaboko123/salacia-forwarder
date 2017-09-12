#include <stdio.h>
#include <stdint.h>
#include "../mynet.h"
#include "checksum.h"

uint16_t checksum_ip(uint8_t *iph){
    int i;
    struct IP *pip;
    uint8_t *p;
    uint16_t tmp;
    uint32_t sum;
    
    sum = 0;
    pip = (struct IP *)iph;
    
    //printf("version : 0x%X\n", pip->version);
    //printf("ip head len : %d byte \n", pip->ihl*4);
    
    p = (uint8_t *)iph;
    for(i = 0; i < pip->ihl*4; i+=2){
        tmp = (uint16_t)(p[i] << 8);
        tmp += p[i+1];
        sum += tmp;
        
        if(sum >> 16 == 1){
            sum = sum - 0x10000 + 0x01;
        }
    }
    
    //printf("sum : 0x%X\n", sum);
    //printf("~sum : 0x%X\n", (~sum) & 0xFFFF);
    
    return((~sum) & 0xFFFF);
}

