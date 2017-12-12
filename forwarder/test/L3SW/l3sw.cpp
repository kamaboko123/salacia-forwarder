#include<cinttypes>
#include <poll.h>
#include "../../src/Ethernet.hpp"
#include "../../src/comlib.hpp"
#include "../../src/dlib.hpp"
#include "../../src/NetIf.hpp"

int main(void){
    //struct pollfd *pfds = new struct pollfd[1];
    
    NetIf netif1((char *)"enp3s0", IPAddress((char *)"172.18.0.15"), IPNetmask((char *)"255.255.255.0"));
    
    //pfds[0].fd = netif1.getFD();
    //pfds[0].events = POLLIN|POLLERR;
    
    Ethernet *pkt = new Ethernet();
    for(;;){
        if(netif1.recv(pkt) == 0) continue;
        dlib::hexdump(pkt->RawData(), pkt->getLength());
        
        if(pkt->getType() == ETHTYPE_ARP){
            printf("[ARP receive]\n");
            printf("src-ip:%s", pkt->arp().getSrcIP().toStr());
        }
        
        printf("\n");
    }
    
    delete pkt;
    
    return(0);
}
