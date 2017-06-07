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
#include <poll.h>
#include "calcaddr.h"

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

struct NETIF{
    char ifname[IFNAMSIZ];
    int pd;
    int ifindex;
    struct ifreq ifr;
    struct sockaddr myaddr;
    struct sockaddr_ll sll;
};

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
    int i, j, s;
    unsigned char buf[2048];
    struct sockaddr *myaddr_p;
    int addrlen;
    struct sockaddr_ll sll;
    
    unsigned int type = 0;
    
    struct ETHER ether;
    struct ETHER *pether;
    
    char tmp[2048];
    int ret;
    
    char ifnames[2][8] ={"enp3s0", "enp4s0"};
    struct NETIF netif[2];
    struct pollfd pfds[2];
    
    for(i = 0; i < 2; i++){
        strcpy(netif[i].ifname, ifnames[i]);
        netif[i].pd = -1;
        if((netif[i].pd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1){
            perror("socket()");
            exit(1);
        }
        //printf("!%d\n", netif[i].pd);
        memset(&netif[i].ifr, 0, sizeof(netif[i].ifr));
        //hexdump(&netif[i].ifr, sizeof(netif[i].ifr));
        strncpy(netif[i].ifr.ifr_name, netif[i].ifname, IFNAMSIZ);
        //hexdump(&netif[i].ifr, sizeof(netif[i].ifr));
        if (ioctl(netif[i].pd, SIOCGIFINDEX, &netif[i].ifr) == -1) {
            perror("SIOCGIFINDEX");
            exit(1);
        }
        netif[i].ifindex = netif[i].ifr.ifr_ifindex;
        //printf("ifindex : %d\n", netif[i].ifindex);
        
        //HWADDR取得
        memset(&netif[i].ifr, 0, sizeof(netif[i].ifr));
        strncpy(netif[i].ifr.ifr_name, netif[i].ifname, IFNAMSIZ);
        if(ioctl(netif[i].pd, SIOCGIFHWADDR, &netif[i].ifr) == -1){
            perror("SIOCGHIFWADDR");
            exit(1);
        }
        netif[i].myaddr = netif[i].ifr.ifr_hwaddr;
        //hexdump(&netif[i].myaddr, sizeof(netif[i].myaddr));
        //myaddr_p = &myaddr;
        //printf("%s\n", netif[i].ifname);
        
        
        //netif[i].sll.sll_addr = netif[i].myaddr;
        
        memset(&netif[i].sll, 0x00, sizeof(netif[i].sll));
        //hexdump((char *)&netif[i].sll, sizeof(netif[i].sll));
        //netif[i].sll.sll_addr[0] = (netif[i].myaddr.sa_data[0]);
        //netif[i].sll.sll_addr[1] = (netif[i].myaddr.sa_data[1]);
        //netif[i].sll.sll_addr[2] = (netif[i].myaddr.sa_data[2]);
        //netif[i].sll.sll_addr[3] = (netif[i].myaddr.sa_data[3]);
        //netif[i].sll.sll_addr[4] = (netif[i].myaddr.sa_data[4]);
        //netif[i].sll.sll_addr[5] = (netif[i].myaddr.sa_data[5]);
        
        //hexdump((char *)&netif[i].sll.sll_addr, sizeof(netif[i].sll.sll_addr));
        
        netif[i].sll.sll_family = AF_PACKET; //allways AF_PACKET
        netif[i].sll.sll_protocol = htons(ETH_P_ALL);
        netif[i].sll.sll_ifindex = netif[i].ifindex;
        
        //hexdump((char *)&netif[i].sll, sizeof(netif[i].sll));
        if (bind(netif[i].pd, (struct sockaddr *)&netif[i].sll, sizeof(netif[i].sll)) == -1) {
            perror("bind():");
            exit(1);
        }
        
        ioctl(netif[i].pd, FIONBIO, 1);
        
        pfds[i].fd = netif[i].pd;
        pfds[i].events = POLLIN|POLLERR;
        
        if (i == 0){
            printf("bridge interfaces : ");
        }
        printf("%s ", netif[i].ifname);
    }
    printf("\n");
    
    for(;;){
        switch(poll(pfds,2,1)){
            case -1:
                perror("polling");
                break;
            case 0:
                break;
            default:
                for(i = 0; i < 2; i++){
                    if(pfds[i].revents&(POLLIN|POLLERR)){
                        if((s=read(netif[i].pd, buf, sizeof(buf))) <= 0){
                            perror("read");
                        }
                        else{
                            pether = (struct ETHER *)buf;
                            type = pether->eth_type[0];
                            type = (type << 8) + pether->eth_type[1];
                            
                            switch (type) {
                                case TYPE_ARP:
                                    //printf("eth_type -> %d[ARP]\n", TYPE_ARP);
                                    break;
                                case TYPE_IP4:
                                    //printf("eth_type -> %d[IPv4]\n", TYPE_IP4);
                                    break;
                                default:
                                    //printf("eth_type -> [Unknown type]\n");
                                    continue;
                                    break;
                            }
                            
                            //printf("[receive]interface:%s\n", netif[i].ifname);
                            //hexdump(buf, s);
                            
                            for (j = 0; j < 2; j++){
                                if(i == j){
                                    continue;
                                }
                                if(write(netif[j].pd, buf, s) <= 0){
                                    perror("read");
                                }
                                //printf("[send]interface:%s\n", netif[j].ifname);
                            }
                            //printf("\n");
                        }
                    }
                }
                break;
        }
    }
    
    return(0);
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
