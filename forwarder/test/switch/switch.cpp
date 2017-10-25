#include <cstdio>
#include <cstdlib>
#include <poll.h>
#include <unistd.h>
#include <cinttypes>
#include <new>

#include "../../src/NetIf.hpp"
#include "../../src/MacAddress.hpp"
#include "../../src/Ethernet.hpp"
#include "../../src/HashMap.hpp"

#define IF_NUM 2

int main(void){
    const int inter_n = IF_NUM;
    
    //perser
    Ethernet packet;
    
    //descriptor for polling
    struct pollfd pfds[IF_NUM];
    
    //Interface
    NetIf netif[2];
    new(netif + 0) NetIf((char *)"enp3s0", IFTYPE_L2_ACCESS, 1);
    new(netif + 1) NetIf((char *)"enp4s0", IFTYPE_L2_ACCESS, 1);
   
    pfds[0].fd = netif[0].pd;
    pfds[0].events = POLLIN|POLLERR;
    
    pfds[1].fd = netif[1].pd;
    pfds[1].events = POLLIN|POLLERR;
    
    //MacAddrTable
    HashMap<MacAddress, NetIf *> mac_tbl(256);
    
    //receive buffer
    uint8_t buf[2048];
    
    //送信先
    NetIf *outif;
    
    for(;;){
        switch(poll(pfds, inter_n, 10)){
            case -1:
                perror("polling");
                break;
            case 0:
                break;
            default:
            for(int i = 0; i < inter_n; i++){
                if(pfds[i].revents&(POLLIN|POLLERR)){
                    //何かしらデータを受けたら
                    int s;
                    if((s = read(netif[i].pd, buf, sizeof(buf))) <= 0){
                        perror("read");
                    }
                    else{
                        packet.set(buf, s);
                        
                        if(packet.getType() == ETHTYPE_UNKNOWN) continue;
                        
                        mac_tbl.update(packet.getSrc(), &netif[i]);
                        outif = mac_tbl.get(packet.getDst());
                        
                        //ブロードキャストか、mac_tblに登録ない場合は全ポートに送信
                        if((packet.getDst().toLong() == 0xFFFFFFFFFFFF) || (outif == nullptr)){
                           for(int j = 0; j < inter_n; j++){
                                if(j == i) continue;
                                netif[j].send(buf, s);
                            }
                           continue;
                        }
                        
                        outif->send(buf, s);
                        
                        /*
                        printf("[size]%d\n", s);
                        printf("[eth]type : %.4x\n", packet.getType());
                        printf("[src]%" PRIx64 "\n", packet.getSrc().toLong());
                        printf("[dst]%" PRIx64 "\n", packet.getDst().toLong());
                        */
                    }
                }
            }
        }
    }
    
    return(0);
}
