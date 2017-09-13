#include <stdio.h>
#include <stdint.h>
#include "../mynet.h"
#include "../swap_endian.h"
#include "checksum.h"


uint16_t checksum_ip(struct IP *ip){
    int i;
    uint8_t *p;
    uint16_t tmp;
    uint32_t sum;
    
    sum = 0;
    
    //printf("version : 0x%X\n", ip->version);
    //printf("ip head len : %d byte \n", ip->ihl*4);
    
    p = (uint8_t *)ip;
    for(i = 0; i < ip->ihl*4; i+=2){
        if (i == 10){
            continue;
        }
        
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

uint16_t checksum_tcp(struct IP *ip){
    struct TCP *tcp;
    struct PSEUDO_IP pip_h;
    int i;
    uint8_t *p;
    uint16_t tmp;
    uint32_t sum;
    
    //疑似ヘッダ
    //pip_h.ip_src = swap_32(ip->ip_src);
    pip_h.ip_src = ip->ip_src;
    //pip_h.ip_dst = swap_32(ip->ip_dst);
    pip_h.ip_dst = ip->ip_dst;
    pip_h.dummy = 0;
    pip_h.ip_proto = 6; //TCP
    
    //疑似ヘッダのパケット長にIPヘッダは含まない
    pip_h.ip_len =  ((ip->packet_len>>8) + (ip->packet_len & 0x00FF)) - (ip->ihl*4);
    //CPUがリトルエンディアンなので変換
    pip_h.ip_len =  swap_16(0x28);
    
    sum = 0;
    tcp = &ip->payload.tcp;
    
    //printf("[tcp]header length:%d(%dbyte)\n", tcp->d_offset, tcp->d_offset*4);
    
    //疑似ヘッダのチェックサム
    p = (uint8_t *)&pip_h;
    for(i = 0; i < sizeof(pip_h); i+=2){
        //printf("|0x%X:%X\n", p[i], p[i+1]);
        
        tmp = (uint16_t)(p[i] << 8);
        tmp += p[i+1];
        
        //printf("tmp:0x%X\n", tmp);
        sum += tmp;
        //printf("sum:0x%X\n", sum);
        
        while(sum >> 16){
            sum = sum - 0x10000 + 0x01;
            //printf("!sum:0x%X\n", sum);
        }
    }
    printf("\n");
    
    //疑似ヘッダのチェックサムにさらにTCPヘッダのチェックサムを足し込む
    p = (uint8_t *)tcp;
    for(i = 0; i < tcp->d_offset*4; i+=2){
        
        tmp = (uint16_t)(p[i] << 8);
        tmp += p[i+1];
        sum += tmp;
        
        while(sum >> 16){
            sum = sum - 0x10000 + 0x01;
        }
    }
    
    return((~sum)&0xFFFF);
}
