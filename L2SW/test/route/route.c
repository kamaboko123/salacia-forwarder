#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../src/calcaddr.h"
#include "../../src/debug.h"

struct PBIT{
    struct PBIT *n_pbit[2];
    uint32_t prefix;
    uint8_t plen;
    uint32_t nexthop;
    uint8_t root;
};

void rtb_init(struct PBIT *root){
    root->n_pbit[0] = NULL;
    root->n_pbit[1] = NULL;
    root->plen = 0;
    root->nexthop = 0;
    root->nexthop = 0;
    root->root = 1;
}

void rtb_add(struct PBIT *root, char *prefix, uint8_t *nexthop){
    uint32_t octet[4];
    uint32_t len;
    uint32_t ip_addr = 0;
    
    struct PBIT *p;
    
    _printf("[add]prefix:%s, nexthop:%s\n", prefix, nexthop);
    
    sscanf(prefix, "%d.%d.%d.%d\n/%d", &octet[0], &octet[1], &octet[2], &octet[3], &len);
    
    for(int i = 0; i < 4; i++){
        ip_addr  += octet[i] << ((3-i)*8);
    }
    //printf("%d\n", ip_addr);
    
    p = root;
    
    for(int i = 0; i < len; i++){
        int bit = (ip_addr & (0x80000000 >> i)) >> (31-i);
        
        //printf("[add]bit(%d): %d\n", bit, i);
        
        if(p->n_pbit[bit] == NULL){
            p->n_pbit[bit] = (struct PBIT *)malloc(sizeof(struct PBIT));
            p->n_pbit[bit]->n_pbit[0] = NULL;
            p->n_pbit[bit]->n_pbit[1] = NULL;
            p->n_pbit[bit]->prefix = 0;
            p->n_pbit[bit]->plen = 0;
            p->n_pbit[bit]->nexthop = 0;
            p->n_pbit[bit]->root = 0;
        }
        
        p = p->n_pbit[bit];
        
        //printf("%u\n", (0x80000000 >> i));
        //printf("[%u]\n", 32-i);
       
        /*
        printf("%u", bit);
        if ((i+1) % 8 == 0){
            printf(".");
        }
        if(i == len - 1){
            printf("\n");
        }
        */
    }
    p->nexthop = (iptoi(nexthop));
    p->prefix = ip_addr;
    p->plen = len;
    
}

uint32_t rtb_get(struct PBIT *root, uint8_t *ip_addr){
    struct PBIT *p;
    uint32_t ip_addr_i;
    uint32_t np = 0;
    
    uint8_t tmp[128] = {0};
    uint8_t tmp2[128] = {0};
    
    p = root;
    ip_addr_i = iptoi(ip_addr);
    
    for(int i = 0; i < 32; i++){
        int bit = (ip_addr_i & (0x80000000 >> i)) >> (31-i);
        //printf("[get]bit: %d\n", bit);
        
        //printf("[%d]np : %s\n", i, itoip(tmp, np));
        if(p->nexthop != 0){
            _printf(" [get-hit]%s/%d -> %s\n", itoip(tmp, p->prefix), p->plen, itoip(tmp2, p->nexthop));
            np = p->nexthop;
        }
        if(p->n_pbit[bit] == NULL){
            return(np);
        }
        p = p->n_pbit[bit];
    }
}

void rtb_dump(struct PBIT *p){
    char tmp1[32];
    char tmp2[32];
    if(p->prefix != 0){
        printf("%s/%d -> %s\n", itoip(tmp1, p->prefix), p->plen, itoip(tmp2, p->nexthop));
    }
    if(p->n_pbit[0] != NULL){
        rtb_dump(p->n_pbit[0]);
    }
    
    if(p->n_pbit[1] != NULL){
        rtb_dump(p->n_pbit[1]);
    }
}


/*
void rtb_dump(struct PBIT *root){
    
    struct PBIT *p;
    
    printf("(root):");
    if (root->n_pbit[0] != NULL){
        p = root->n_pbit[0];
        while(p->n_pbit)
        printf("\n");
    }
}*/

int main(void){
    struct PBIT root;
    rtb_init(&root);
    
    uint8_t s[128];
    
    rtb_add(&root, "10.0.0.0/16", "1.1.1.1");
    rtb_add(&root, "10.0.0.0/8", "2.2.2.2");
    rtb_add(&root, "10.0.1.0/24", "3.3.3.3");
    rtb_add(&root, "10.0.2.0/24", "4.4.4.4");
    rtb_add(&root, "1.0.0.0/24", "5.5.5.5");
    rtb_add(&root, "192.0.1.0/24", "6.6.6.6");
    
    itoip(s, rtb_get(&root, "10.0.0.1"));
    printf("[get]addr=%s, nexthop=%s\n", "10.0.0.1", s);
    
    itoip(s, rtb_get(&root, "10.1.1.1"));
    printf("[get]addr=%s, nexthop=%s\n", "10.1.1.1", s);
    
    
    //rtb_add(&root, "10.1.1.0/16");
    //rtb_add(&root, "10.1.2.0/24");
    rtb_dump(&root);
    
    return(0);
}

