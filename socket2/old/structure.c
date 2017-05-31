#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define SIZE 32

void make_arp_req(struct ETHER *ether, uint8_t *src_mac, uint8_t *src_ip, uint8_t *target_ip);
void nts(unsigned char *str, unsigned long long int n, int bytes);
void hexdump(uint8_t *p, int count);

struct ARP{
    uint8_t hw_type[2];
    uint8_t proto_type[2];
    uint8_t hlen[1];
    uint8_t plen[1];
    uint8_t op_code[2];
    uint8_t src_mac[6];
    uint8_t src_ip[4];
    uint8_t dst_mac[6];
    uint8_t dst_ip[4];
} __attribute__((__packed__));

struct ETHER{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint8_t eth_type[2];
    
    union {
        struct ARP arp;
    }payload;
    
} __attribute__((__packed__));


int main(void)
{
    struct ETHER ether;
    struct ETHER *pether;
    
    pether = &ether;
    
    //uint8_t dst[] = "112233445566";
    //printf("!!!%s\n", dst);
    printf("!!!%llx\n", (uint64_t)strtoll("112233445566", NULL, 16));
    
    //sscanf("111111222222", "%x", ether.dst_mac);
    nts(ether.dst_mac, strtoll("FFFFFFFFFFFF", NULL, 16), 6);
    nts(ether.src_mac, strtoll("112233445566", NULL, 16), 6);
    nts(ether.eth_type, 0x0806, 2);
    
    printf("[%.2x]\n", ether.dst_mac);
    printf("[%.2x]\n", pether->dst_mac);
    hexdump((uint8_t *)&ether, 50);
    return(0);
}

void make_arp_req(struct ETHER *ether, uint8_t *src_mac, uint8_t *src_ip, uint8_t *target_ip)
{
    int i;
}

void hexdump(uint8_t *p, int count)
{
    int i, j;

    for(i = 0; i < count; i += 16) {
        printf("%04x : ", i);
        for (j = 0; j < 16 && i + j < count; j++)
            printf("%2.2x ", p[i + j]);
        for (; j < 16; j++) {
            printf("   ");
        }
        printf(": ");
        for (j = 0; j < 16 && i + j < count; j++) {
            char c = toascii(p[i + j]);
            printf("%c", isalnum(c) ? c : '.');
        }
        printf("\n");
    }
}

/*
void hexdump(uint_8t *str, int bytes)
{
    int i;
    int j;
    
    for(i=0; bytes / 8 <= i; i*=8){
        for i =
        printf("%.2x");
    }
}*/

/*
int main(void)
{
    unsigned char str[SIZE] = "ab";
    int i;
    
    // 0x0806
    
    printf("%s\n", str);
    printf("%.2x %.2x\n", str[0], str[1]);
    
    nts(str , (unsigned int)0x0806, SIZE);
    printf("%s\n", str);
    printf("%.2x %.2x\n", str[0], str[1]);
    printf("%.2x %.2x\n", &str[0], &str[1]);
    printf("---\n");
    printf("%.2x %.2x\n", *str, *(str+1));
    printf("%.2x %.2x\n", str, (str+1));
    printf("---\n");
    printf("%.2x %.2x\n", *str, *(str)+2);
    printf("%p %p\n", str, str+2);
    printf("---\n");
    
    for(i=0; i<SIZE; i++){
        printf("%.2x ", str[i]);
    }
    printf("\n");
}
*/
void nts(unsigned char *str, unsigned long long int n, int bytes){
    int i, j;
    int shift;
    
    for(i=1; i<=bytes; i++){
        shift = (bytes-i) * 8;
        str[i-1] = (unsigned char)(n >> shift);
        printf("shift:%d -> %.2x\n", shift, (unsigned char)(n >> shift));
        //printf("!%d : %.2x\n", bytes-i, (unsigned char)(n >> shift));
    }
    printf("?%.2x\n", (unsigned char)(n>>8));
    printf("?%.2x\n", (unsigned char)(n>>0));
}
