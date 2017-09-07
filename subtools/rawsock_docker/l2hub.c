#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <poll.h>


char *interface = NULL;
int pd = -1;

#define TYPE_ARP 0x0806
#define TYPE_IP4 0x0800

#define IFMAX 4

//ARP structure
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

//Ether frame structure
struct ETHER{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint8_t eth_type[2];
    
    union {
        struct ARP arp;
    }payload;
    
} __attribute__((__packed__));

//network interface
struct NETIF{
    char ifname[IFNAMSIZ];
    int pd;
    int ifindex;
    struct ifreq ifr;
    struct sockaddr myaddr;
    struct sockaddr_ll sll;
};

void hexdump(unsigned char *buf, int nbytes);

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
    
    int inter_n;
    
    char ifnames[IFMAX][IFNAMSIZ];
    struct NETIF netif[IFMAX];
    struct pollfd pfds[IFMAX];
    
    //引数からbridgeするinterfaceを取ってくる
    inter_n = argc - 1;
    if(inter_n <= 1){
        fprintf(stdout, "Args Error : You need specify 2 interfaces at leaset.\n");
        exit(-1);
    }
    if(inter_n > IFMAX){
        fprintf(stdout, "Args Error : Too many interfaces. IFMAX : %d.\n", IFMAX);
        exit(-1);
    }
    for(i = 0; i < inter_n; i++){
        if(strlen(argv[i+1]) > IFNAMSIZ){
            fprintf(stdout, "Invaild Args.\n");
            exit(-1);
        }
        strcpy(ifnames[i], argv[i+1]);
    }
    
    //raw socketで扱うために色々やる
    for(i = 0; i < inter_n; i++){
        
        //socket作る
        strcpy(netif[i].ifname, ifnames[i]);
        netif[i].pd = -1;
        if((netif[i].pd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1){
            perror("socket()");
            exit(1);
        }
        
        //interfaceの名前からifindexを取ってくる
        memset(&netif[i].ifr, 0, sizeof(netif[i].ifr));
        strncpy(netif[i].ifr.ifr_name, netif[i].ifname, IFNAMSIZ);
        if (ioctl(netif[i].pd, SIOCGIFINDEX, &netif[i].ifr) == -1) {
            perror("SIOCGIFINDEX");
            exit(1);
        }
        netif[i].ifindex = netif[i].ifr.ifr_ifindex;
        
        //HWADDR取得
        memset(&netif[i].ifr, 0, sizeof(netif[i].ifr));
        strncpy(netif[i].ifr.ifr_name, netif[i].ifname, IFNAMSIZ);
        if(ioctl(netif[i].pd, SIOCGIFHWADDR, &netif[i].ifr) == -1){
            perror("SIOCGHIFWADDR");
            exit(1);
        }
        netif[i].myaddr = netif[i].ifr.ifr_hwaddr;
        
        memset(&netif[i].sll, 0x00, sizeof(netif[i].sll));
        
        //socketにinterfaceをbind
        netif[i].sll.sll_family = AF_PACKET; //allways AF_PACKET
        netif[i].sll.sll_protocol = htons(ETH_P_ALL);
        netif[i].sll.sll_ifindex = netif[i].ifindex;
        
        if (bind(netif[i].pd, (struct sockaddr *)&netif[i].sll, sizeof(netif[i].sll)) == -1) {
            perror("bind():");
            exit(1);
        }
        
        //ノンブロッキングモードに
        ioctl(netif[i].pd, FIONBIO, 1);
        
        //poll()を使うのでそっちで使う構造体にもデータ渡しておく
        pfds[i].fd = netif[i].pd;
        pfds[i].events = POLLIN|POLLERR;
        
        if (i == 0){
            printf("bridge interfaces : ");
        }
        printf("%s ", netif[i].ifname);
    }
    printf("\n");
    
    for(;;){
        switch(poll(pfds, inter_n, 10)){
            case -1:
                perror("polling");
                break;
            case 0:
                break;
            default:
                //イベントを受けたらパケット処理
                for(i = 0; i < inter_n; i++){
                    if(pfds[i].revents&(POLLIN|POLLERR)){
                        //何かしらデータを受けたら
                        if((s=read(netif[i].pd, buf, sizeof(buf))) <= 0){
                            perror("read");
                        }
                        else{
                            //受信したデータをdump(debug)
                            
                            pether = (struct ETHER *)buf;
                            type = pether->eth_type[0];
                            type = (type << 8) + pether->eth_type[1];
                            
                            printf("[receive]interface:%s\n ", netif[i].ifname);
                            switch (type) {
                                case TYPE_ARP:
                                    printf("eth_type -> %d[ARP]\n", TYPE_ARP);
                                    break;
                                case TYPE_IP4:
                                    printf("eth_type -> %d[IPv4]\n", TYPE_IP4);
                                    break;
                                default:
                                    printf("eth_type -> [Unknown type]\n");
                                    continue;
                                    break;
                            }
                            
                            //hexdump(buf, s);
                            
                            //受信したinterface以外のinterfaceに送信する
                            for (j = 0; j < inter_n; j++){
                                if(i == j){
                                    continue;
                                }
                                if(write(netif[j].pd, buf, s) <= 0){
                                    perror("read");
                                }
                                printf("[send]interface:%s\n", netif[j].ifname);
                            }
                            printf("\n");
                        }
                    }
                }
                break;
        }
    }
    
    return(0);
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

