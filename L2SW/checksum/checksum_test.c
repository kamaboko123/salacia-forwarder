#include <stdio.h>
#include <stdint.h>
#include "checksum.h"

int main(void){
    uint16_t ip_csum;
    
    uint8_t packet[] = {
        0x45, 0x00, 0x16, 0xce, 0x65, 0x4c, 0x40, 0x00, 0x01, 0x11, 0xf7, 0xeb, 0xc0, 0xa8, 0x65, 0x1f, 0xe0, 0x00, 0x00, 0x1f
    };
    //checksum : 0xf7eb
    
    ip_csum = checksum_ip(packet);
    printf("[IP checksum]\n");
    printf("0x%X -> %s\n", ip_csum, (ip_csum == 0xF7EB) ? "correct" : "incorrect");
    
    return(0);
}
