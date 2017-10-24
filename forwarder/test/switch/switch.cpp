#include <cstdio>
#include <cstdlib>
#include <poll.h>
#include <unistd.h>
#include <cinttypes>
#include <new>

#include "../../src/NetIf.hpp"
#include "../../src/MacAddress.hpp"
#include "../../src/Ethernet.hpp"

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
    
    
    //receive buffer
    uint8_t buf[2048];
    
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
                        
                        /*
                        printf("[size]%d\n", s);
                        printf("[eth]type : %.4x\n", packet.getType());
                        printf("[src]%" PRIx64 "\n", packet.getSrc().toLong());
                        printf("[dst]%" PRIx64 "\n", packet.getDst().toLong());
                        */
                        
                        //全ポートに送信
                        for(int j = 0; j < inter_n; j++){
                            if(j == i) continue;
                            netif[j].send(buf, s);
                        }
                    }
                }
            }
        }
    }
    
    return(0);
}
