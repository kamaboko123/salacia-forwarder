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
#include "mynet.h"
#include "mac_table.h"
#include "debug.h"

char *interface = NULL;
int pd = -1;

#define IFMAX 4
#define MAC_REF_INTERVAL 10

//network interface
struct NETIF{
    char ifname[IFNAMSIZ];
    int pd;
    int ifindex;
    struct ifreq ifr;
    struct sockaddr myaddr;
    struct sockaddr_ll sll;
};

void hexdump(uint8_t *buf, int nbytes);

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
    
    uint32_t mac_ref_time_last = 0;
    
    //mac-address-table
    struct MAC_TABLE mac_table = {0};
    
    //mac-address-table 初期化
    init_mac_table(&mac_table);
    mac_ref_time_last = time(NULL);
    
    //引数からbridgeするinterfaceを取ってくる
    inter_n = argc - 1;
    if(inter_n <= 1){
        fprintf(stderr, "Args Error : You need specify 2 interfaces at leaset.\n");
        exit(-1);
    }
    if(inter_n > IFMAX){
        fprintf(stderr, "Args Error : Too many interfaces. IFMAX : %d.\n", IFMAX);
        exit(-1);
    }
    for(i = 0; i < inter_n; i++){
        if(strlen(argv[i+1]) > IFNAMSIZ){
            fprintf(stderr, "Invaild Args.\n");
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
        if (time(NULL) - mac_ref_time_last >= MAC_REF_INTERVAL){
            _printf("\n//////////[[refresh mac table]]//////////\n");
            refresh_mac_table(&mac_table);
            mac_ref_time_last = time(NULL);
            _dump_mac_table(&mac_table);
            _printf("//////////////////////////////////////////\n");
        }
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
                            
                            //フレーム処理用の変数
                            uint8_t tmp_dst[32] = {0};
                            uint8_t tmp_src[32] = {0};
                            struct MAC_TABLE_ENTRY *sw_dst;
                            struct MAC_TABLE_ENTRY *sw_src;
                            
                            
                            //受信したデータをdump(debug)
                            pether = (struct ETHER *)buf;
                            type = pether->eth_type[0];
                            type = (type << 8) + pether->eth_type[1];
                            
                            _printf("[receive]interface:%s\n", netif[i].ifname);
                            switch (type) {
                                case TYPE_ARP:
                                    _printf("eth_type -> 0x%.2x%.2x[ARP]\n", pether->eth_type[0], pether->eth_type[1]);
                                    break;
                                case TYPE_IP4:
                                    _printf("eth_type -> 0x%.2x%.2x[IPv4]\n", pether->eth_type[0], pether->eth_type[1]);
                                    break;
                                default:
                                    _printf("eth_type -> 0x%.2x%.2x[Unknown type]\n", pether->eth_type[0], pether->eth_type[1]);
                                    continue;
                                    break;
                            }
                            
                            _hexdump(buf, s);
                            
                            //送信元MAC、Table更新
                            ltomac(tmp_src, mactol_bin(pether->src_mac));
                            _printf("src:%s\n", tmp_src);
                            update_mac_table(&mac_table, tmp_src, netif[i].ifindex);
                            
                            //送信先MAC
                            ltomac(tmp_dst, mactol_bin(pether->dst_mac));
                            _printf("dst:%s\n", tmp_dst);
                            
                            //mac-address-tableのdump
                            _dump_mac_table(&mac_table);
                            
                            //ブロードキャストは受信したinterface以外のinterfaceに送信する
                            _hexdump((uint8_t *)pether, 14);
                            //printf("%"PRIu64"\n", (uint64_t)mactol_bin(pether->dst_mac));
                            if (mactol_bin(pether->dst_mac) == 0xFFFFFFFFFFFF){
                                _printf("broad-cast packet\n");
                                for (j = 0; j < inter_n; j++){
                                    if(i == j){
                                        continue;
                                    }
                                    if(write(netif[j].pd, buf, s) <= 0){
                                        perror("send");
                                    }
                                    _printf("[send]interface:%s\n", netif[j].ifname);
                                }
                            }
                            else{
                                //_hexdump(tmp_dst ,30);
                                
                                sw_dst = get_mac_entry(&mac_table, tmp_dst);
                                
                                if(sw_dst == NULL){
                                    _printf("!send all ports\n");
                                    for (j = 0; j < inter_n; j++){
                                        if(i == j){
                                            continue;
                                        }
                                        if(write(netif[j].pd, buf, s) <= 0){
                                            perror("send");
                                        }
                                        _printf("[send]interface:%s\n", netif[j].ifname);
                                    }
                                }
                                else{
                                    int k;
                                    for(k = 0; k < inter_n; k++){
                                        if(netif[k].ifindex == sw_dst->netif_index){
                                            if(write(netif[k].pd, buf, s) <= 0){
                                                perror("send");
                                            }
                                            _printf("[send]interface:%s\n", netif[k].ifname);
                                            break;
                                        }
                                    }
                                }
                                _printf("non-broad-cast packet\n");
                            }
                            _printf("-------------------------\n");
                        }
                    }
                }
                break;
        }
    }
    
    return(0);
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

