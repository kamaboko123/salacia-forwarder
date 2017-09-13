#include "calcaddr.h"

uint32_t iptoi(const char *addr){
    //アドレス形式例 : 192.168.1.1
    //末尾にNULL文字必要
    const char *p = addr;
    uint32_t dec = 0;
    int i;

    for(i = 0; i < 4; i++) {
        uint32_t v = 0;
        while (*p != '\0' && *p != '.') {
            v = v * 10 + (*p - '0');
            ++p;
        }
        if (*p == '.') {
            ++p;
        }
        dec = (dec << 8) | v;
    }

    return(dec);
}

uint8_t *ltomac(uint8_t *ret, uint64_t addr_l){
    //uint8_t ret[18] = {0};
    uint8_t tmp[13] = {0};
    int i;
    int j = 0;
    
    sprintf(tmp, "%012lx", addr_l);
    for(i = 1; tmp[i-1] != '\0'; i++){
        ret[j] = tmp[i-1];
        if((i > 0) && (i % 2 == 0)){
            j++;
            ret[j] = ':';
        }
        j++;
        //printf("%s", tmp);
    }
    ret[j-1] = '\0';
    //printf("%s\n", ret);
    return(ret);
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

uint64_t mactol_bin(uint8_t *addr){
    //アドレス形式例 : 112233aabbcc
    //末尾にNULL文字不要
    //6octet -> uint64t
    
    int mac_octet = 6;
    uint64_t ret = 0;
    int i = 0, j = 11;
   
    for(i = 1; i <= mac_octet; i++){
        //printf("%u, %d\n", addr[i-1], (8*(mac_octet-i)));
        ret += (uint64_t)addr[i-1] << (8*(mac_octet-i));
        //printf("%"PRIu64"\n", ret);
    }
    //uprintf("%"PRIu64"\n", ret);
    return(ret);
    
    /*
    for(i = 0; i < 12; i++){
        printf("%x\n", addr[i]);
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
    printf("\n");
    //printf("%" PRId64 "\n", ret);
    return(ret);
    */
}

