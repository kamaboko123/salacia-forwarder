#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if_arp.h>

char *interface = NULL;
int pd = -1;

#define TYPE_ARP 0x0806
#define TYPE_IP4 0x0800

typedef struct eth_h{
    unsigned char dst_addr[6];
    unsigned char src_addr[6];
    unsigned char type[2];
    struct ip_h *ip_h_p;
} __attribute__ ((packed)) eth_h;

typedef struct ip_h{
    unsigned char ver[1];
    //TBD
} __attribute__ ((packed)) ip_h;


void sigint(int);
void hexdump(unsigned char *buf, int nbytes);
void strnget(char *buf, char *str, int n);

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
    
    struct eth_h *eth_h_p;
    
    unsigned int type = 0;
    
    /*tmp var*/
    char hwaddr_tmp[2048];
    
    interface = strdup("enp4s0");
    
    //割り込み
    signal(SIGINT, sigint);
    
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
            printf("family:default(%d, %d)\n", myaddr.sa_family, sizeof(myaddr.sa_data));
            addrlen = sizeof(myaddr.sa_data);
    }
    
    //bind interface
    //sll = sockadrr_ll = socket address link layer?
    
    memset(&sll, 0xff, sizeof(sll));
    sll.sll_family = AF_PACKET; //allways AF_PACKET
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = ifindex;
    if (bind(pd, (struct sockaddr *)&sll, sizeof sll) == -1) {
        perror("bind():");
        exit(1);
    }
    
    
    
    //Receive
    for (;;) {
        i = recv(pd, buf, sizeof(buf), 0);
        if (i < 0) {
            perror("recv():");
            exit(1);
        }
        if (i == 0){
            continue;
        }
        
        eth_h_p = (struct eth_h *)buf;
        //printf("%d\n", sizeof(eth_h_p->dst_addr));
        
        printf("dst_addr -> ");
        for(j = 0; j < sizeof(eth_h_p->dst_addr); j++){
            printf("%.2x%c", eth_h_p->dst_addr[j], (j < sizeof(eth_h_p->dst_addr)-1?':':'\n'));
        }
        
        printf("src_addr -> ");
        for(j = 0; j < sizeof(eth_h_p->src_addr); j++){
            printf("%.2x%c", eth_h_p->src_addr[j], (j < sizeof(eth_h_p->src_addr)-1?':':'\n'));
        }
        
        
        type = eth_h_p->type[0];
        type = (type << 8) + eth_h_p->type[1];
        
        switch (type) {
            case TYPE_ARP:
                printf("eth_type -> %d[ARP]\n", TYPE_ARP);
                break;
            
            case TYPE_IP4:
                printf("eth_type -> %d[IPv4]\n", TYPE_IP4);
                break;
            
            default:
                printf("eth_type -> [Unknown type]\n");
                break;
        }
        
        hexdump(buf, i);
    }
    
    /* NOT REACHED */
    exit(0);
}

void sigint(signum)
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
