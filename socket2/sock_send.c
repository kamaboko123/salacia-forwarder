#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if_arp.h>
#include <sys/types.h>
#include <netinet/in.h>

char *interface = NULL;
int pd = -1;

#define TYPE_ARP 0x0806
#define TYPE_IP4 0x0800

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
    uint8_t padding[18];
} __attribute__((__packed__));

struct ETHER{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint8_t eth_type[2];
    
    union {
        struct ARP arp;
    }payload;
    
    //uint8_t padding[18];
} __attribute__((__packed__));


void sigint(int signum);
void hexdump(unsigned char *buf, int nbytes);
void strnget(char *buf, char *str, int n);
void nts(unsigned char *str, unsigned long long int n, int bytes);

void strnget(char *buf, char *str, int n){
    memcpy(buf, str, n);
    buf[n] = '\0';
}

int main(int argc, char **argv)
{
    struct ifreq ifr;
    int ifindex;
    struct sockaddr myaddr;
    int i, j;
    unsigned char buf[2048];
    struct sockaddr *myaddr_p;
    int addrlen;
    struct sockaddr_ll sll;
    
    unsigned int type = 0;
    
    struct ETHER ether;
    struct ETHER *pether;
    
    char tmp[2048];
    int ret;
    
    /*tmp var*/
    char hwaddr_tmp[2048];
    
    interface = strdup("enp4s0");
    
    //割り込み
    //signal(SIGINT, sigint);
    
    //socket
    pd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (pd == -1) {
        perror("socket():");
        exit(1);
    }
    printf("socket_pd : %d\n", pd);
    
    //ifindex取得
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);
    if (ioctl(pd, SIOCGIFINDEX, &ifr) == -1) {
        perror("SIOCGIFINDEX");
        exit(1);
    }
    ifindex = ifr.ifr_ifindex;
    printf("ifindex : %d\n", ifindex);
    
    //HWADDR取得
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);
    if (ioctl(pd, SIOCGIFHWADDR, &ifr) == -1){
        perror("SIOCGHIFWADDR");
        exit(1);
    }
    myaddr = ifr.ifr_hwaddr;
    myaddr_p = &myaddr;
    
    hexdump((unsigned char *)myaddr_p, sizeof(myaddr));
    //printf("\n\n");
    //hexdump((unsigned char *)strnget(myaddr_p->sa_data, 14), 2048);
    //strnget(hwaddr_tmp, myaddr_p->sa_data, 14);
    //hexdump((unsigned char *)hwaddr_tmp, 2048);
    //printf("hwaddr : %.2x, %x \n", (unsigned char)myaddr_p->sa_family, (unsigned char *)hwaddr_tmp);
    printf("hwaddr : %.2x, %.2x:%.2x:%.2x:%.2x:%.2x:%.2x \n", 
            (unsigned char)myaddr_p->sa_family,
            (unsigned char)myaddr_p->sa_data[0],
            (unsigned char)myaddr_p->sa_data[1],
            (unsigned char)myaddr_p->sa_data[2],
            (unsigned char)myaddr_p->sa_data[3],
            (unsigned char)myaddr_p->sa_data[4],
            (unsigned char)myaddr_p->sa_data[5]);
    
    switch (myaddr.sa_family) {
        case ARPHRD_ARCNET:
            addrlen = 1;
            printf("family:ARPHRD_ARCNET(%d)\n", ARPHRD_ARCNET);
            break;
        case ARPHRD_ETHER:
            printf("family:ARPHRD_ETHER(%d)\n", ARPHRD_ETHER);
            addrlen = 6;
            break;
        default:
            printf("family:default(%d, %lu)\n", myaddr.sa_family, sizeof(myaddr.sa_data));
            addrlen = sizeof(myaddr.sa_data);
    }
    
    //bind interface
    //sll = sockadrr_ll = socket address link layer?
    
    //memset(&sll, 0xff, sizeof(sll));
    sll.sll_addr[0] = 0x00;
    sll.sll_addr[1] = 0x3d;
    sll.sll_addr[2] = 0x2c;
    sll.sll_addr[3] = 0x15;
    sll.sll_addr[4] = 0x24;
    sll.sll_addr[5] = 0x57;
    sll.sll_family = AF_PACKET; //allways AF_PACKET
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = ifindex;
    if (bind(pd, (struct sockaddr *)&sll, sizeof sll) == -1) {
        perror("bind():");
        exit(1);
    }
    
    printf("%.2x %2.x %.2x %.2x %.2x %.2x \n",
        sll.sll_addr[0],
        sll.sll_addr[1],
        sll.sll_addr[2],
        sll.sll_addr[3],
        sll.sll_addr[4],
        sll.sll_addr[5]
    );
    
    //Send ARP
    for(;;){
        pether = &ether;
        memset(&ether, 0, sizeof(ether));
        
        nts(ether.dst_mac, strtoll("FFFFFFFFFFFF", NULL, 16), 6);
        nts(ether.src_mac, strtoll("003d2c152457", NULL, 16), 6);
        //for(i=0; i<18; i++){
        //    ether.payload.arp.padding[i] = 0;
        //}
        nts(ether.eth_type, 0x0806, 2);
        
        //arp request
        //*ether.payload.arp.hw_type = (uint16_t)0x1122;
        nts(ether.payload.arp.hw_type, strtoll("0001", NULL, 16), 2);
        //ether.payload.arp.hw_type[0] = (uint8_t)0x00;
        //ether.payload.arp.hw_type[0] = (uint8_t)0x01;
        //*ether.payload.arp.proto_type = (uint16_t)0x0800;
        nts(ether.payload.arp.proto_type, strtoll("0800", NULL, 16), 2);
        nts(ether.payload.arp.hlen, strtoll("6", NULL, 10), 1);
        nts(ether.payload.arp.plen, strtoll("4", NULL, 10), 1);
        //*ether.payload.arp.hlen = (uint8_t)48;
        //*ether.payload.arp.plen = (uint8_t)32;
        //*ether.payload.arp.op_code = (uint16_t)1;
        nts(ether.payload.arp.op_code, strtoll("0001", NULL, 16), 2);
        nts(ether.payload.arp.src_mac, strtoll("003d2c152457", NULL, 16), 6);
        nts(ether.payload.arp.src_ip, strtoll("c0a81e02", NULL, 16), 4);
        nts(ether.payload.arp.dst_mac, strtoll("000000000000", NULL, 16), 6);
        nts(ether.payload.arp.dst_ip, strtoll("c0a81e01", NULL, 16), 4);
        
        
        hexdump((unsigned char *)&ether, sizeof(ether));
        printf("%d bytes\n", sizeof(ether));
        
        ret = sendto(pd, &ether, 60, 0, (struct sockaddr *)&sll, sizeof(sll));
        
        printf("!!%d!!\n", ret);
        
        sleep(1);
    }
    
    /* NOT REACHED */
    exit(0);
}

void sigint(int signum)
//int signum;
{
    printf("signum:%d\n", signum);
    
    struct ifreq ifr;

    if (pd == -1)
        return;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);
    ioctl(pd, SIOCGIFFLAGS,&ifr);
    ifr.ifr_flags &= ~IFF_PROMISC;
    ioctl(pd, SIOCSIFFLAGS,&ifr);

    close(pd);
    exit(0);
}

void hexdump(unsigned char *p, int count)
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
