#include<cinttypes>
#include <poll.h>
#include "Ethernet.hpp"
#include "comlib.hpp"
#include "dlib.hpp"
#include "NetIf.hpp"

void reply_arp(NetIf &netif, Ethernet &pkt){
    uint8_t arp_base[] = PKT_ARP;
    Ethernet arp_reply(arp_base, PKT_ARP_SIZE);
    
    arp_reply.arp().setSrcMac(netif.getMac());
    arp_reply.arp().setDstMac(pkt.arp().getSrcMac());
    
    arp_reply.arp().setSrcIP(netif.getIP());
    arp_reply.arp().setDstIP(pkt.arp().getSrcIP());
    
    arp_reply.arp().setOPCode(ARPOPCode::ARP_REPLY);
    arp_reply.setDst(pkt.arp().getSrcMac());
    
    printf("[Send ARP reply]");
    printf("%s is at %" PRIx64  "\n", netif.getIP().toStr(), netif.getMac().toLong());
    netif.send(arp_reply);
}


int main(void){
    //struct pollfd *pfds = new struct pollfd[1];
    
    NetIf netif1((char *)"11h", IPAddress((char *)"172.18.0.15"), IPNetmask((char *)"255.255.255.0"));
    printf("Interface Name: 11h\n");
    printf("Interface IPAddr: 172.18.0.15\n");
    printf("Interface MacAddr: %" PRIx64 "\n", netif1.getMac().toLong());
    
    //pfds[0].fd = netif1.getFD();
    //pfds[0].events = POLLIN|POLLERR;
    
    Ethernet *pkt = new Ethernet();
    for(;;){
        if(netif1.recv(pkt) == 0) continue;
        dlib::hexdump(pkt->RawData(), pkt->getLength());
        
        if(pkt->isARP()){
            printf("[receive ARP request]\n");
            printf("Who has %s ? [sender: %s(%" PRIx64 ")]\n", pkt->arp().getDstIP().toStr(), pkt->arp().getSrcIP().toStr(), pkt->arp().getSrcMac().toLong());
            
            if(pkt->arp().getOPCode() == ARPOPCode::ARP_REQUEST && pkt->arp().getDstIP() == IPAddress((char *)"172.18.0.15")){
                reply_arp(netif1, *pkt);
            }
        }
        
        else if(pkt->isIPv4()){
            printf("[receive IP Packet]\n");
        }
        
        printf("\n");
    }
    
    delete pkt;
    
    return(0);
}
