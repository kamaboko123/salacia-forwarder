#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

unsigned int iptoi(char *addr);
uint64_t mactol(uint8_t *addr);


unsigned int iptoi(char *addr){
    //アドレス形式例 : 192.168.1.1
    //末尾にNULL文字必要
    
    //char addr[] = "1.1.1.1";
    unsigned char octet[4][4];
    int i= 0, j = 0, k = 0;
    char buf[4];
    unsigned int dec = 0;
    
    for(i=0; i < 4; i++){
        for(j=0; j < 4; j++){
            octet[i][j] = 0;
            //printf("%d\n", octet[i][j]);
        }
    }
    
    j = 0;
    k = 0;
    for (i=0; addr[i] != '\0'; i++){
        if (addr[i] == '.'){
            octet[j][k] = '\0';
            j++;
            k = 0;
            continue;
        }
        octet[j][k] = addr[i];
        k++;
    }
    
    for(i=0; i < 4; i++){
        dec += atoi(octet[i]) << 8*(3-i);
        //printf("%s %u\n", octet[i], dec);
        /*
        if(i < 3){
            dec = dec << 8;
        }*/
    }
    
    //printf("%s\n", addr);
    //printf("!%u\n", dec);
    return(dec);
}

uint64_t mactol(uint8_t *addr){
    //アドレス形式例 : 11:22:33:aa:bb:cc
    //末尾にNULL文字必要
    
    uint64_t ret = 0;
    int i = 0, j = 11;
    
    for(i = 0; addr[i] != '\0'; i++){
        if(addr[i] != ':'){
            if((addr[i] >= '0') && (addr[i] <= '9')){
                ret += (uint64_t)(addr[i] - '0') << j * 4;
            }
            else if((addr[i] >= 'A') && (addr[i] <= 'F')){
                ret += (uint64_t)(addr[i] - 'A' + 10) << j * 4;
            }
            else if((addr[i] >= 'a') && (addr[i] <= 'f')){
                ret += (uint64_t)(addr[i] - 'a' + 10) << j * 4;
            }
            
            j--;
        }
    }
    
    //printf("%" PRId64 "\n", ret);
    return(ret);
}


