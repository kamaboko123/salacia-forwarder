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
#include "../../src/comlib.hpp"
#include "../../src/dlib.hpp"

#define IFMAX 4

int main(int argc, char **argv){
    int inter_n = 0;
    
    //perser
    //Ethernet packet;
    
    inter_n = argc - 1;
    if(inter_n <= 1){
        fprintf(stderr, "Args Error : You need specify 2 interfaces at leaset.\n");
        exit(-1);
    }
    if(inter_n > IFMAX){
        fprintf(stderr, "Args Error : Too many interfaces. IFMAX : %d.\n", IFMAX);
        exit(-1);
    }
    
    //descriptor for polling
    struct pollfd *pfds = new struct pollfd[inter_n];
    NetIf *netif = new NetIf[inter_n];
    
    for(int i = 0; i < inter_n; i++){
        if(comlib::strlen(argv[i + 1]) > IFNAMSIZ){
            fprintf(stderr, "Invaild Args.\n");
            exit(-1);
        }
        if(i == 0){
            //first interface is trunk port
            new(netif + i) NetIf(argv[i + 1], IFTYPE_L2_TRUNK, 0);
            printf("set : %s(t)\n", netif[i].getIfName());
        }
        else{
            new(netif + i) NetIf(argv[i + 1], IFTYPE_L2_ACCESS, 3100);
            printf("set : %s(a)\n", netif[i].getIfName());
        }
        pfds[i].fd = netif[i].getFD();
        pfds[i].events = POLLIN|POLLERR;
    }
    
    //MacAddrTable
    //HashMap<MacAddress, NetIf *> mac_tbl(256);
    
    //receive buffer
    //uint8_t buf[2048];
    Ethernet pbuf;
    
    /*
    for(int j = 0; j < inter_n; j++){
        printf("%s %d\n", netif[j].getIfName(), netif[j].getIfType());
    }
    */
    
    //送信先
    //NetIf *outif;
    
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
                    if(netif[i].recv(&pbuf) == 0) continue;
                    
                    for(int j = 0; j < inter_n; j++){
                        if(j == i) continue;
                        netif[j].send(pbuf, netif[i].getVlanId());
                    }
                    
                    /*
                    dlib::hexdump((uint8_t *)packet.RawData(), packet.getLength());
                    Ethernet tag(buf, s);
                    Ethernet untag(buf, s);
                    
                    if(netif[i].getIfType() == IFTYPE_L2_ACCESS){
                        tag.setVlanTag(netif[i].getVlanId());
                    }
                    else if(netif[i].getIfType() == IFTYPE_L2_TRUNK){
                        untag.removeVlanTag();
                    }
                    
                    if(s > 1500){
                        printf("too long : %d\n", s);
                        continue;
                    }
                    
                    printf("recv : %s\n", netif[i].getIfName());
                    printf("type : %.04x\n", packet.getType());
                    printf("len : %dbyte\n", packet.getLength());
                    
                    for(int j = 0; j < inter_n; j++){
                        if(j == i) continue;
                        
                        printf("to_name:<%s>\n", netif[j].getIfName());
                        printf("to_type<%d>\n", netif[j].getIfType());
                        if(netif[j].getIfType() == IFTYPE_L2_ACCESS){
                            netif[j].sendRaw(untag.RawData(), untag.getLength());
                            printf("send to access\n");
                            dlib::hexdump((uint8_t *)untag.RawData(), untag.getLength());
                        }
                        else if(netif[j].getIfType() == IFTYPE_L2_TRUNK){
                            netif[j].sendRaw(tag.RawData(), tag.getLength());
                            printf("send to trunk(%dbyte)\n", tag.getLength());
                            dlib::hexdump((uint8_t *)tag.RawData(), tag.getLength());
                        }
                    }*/
                    /*
                    printf("----\n");
                    
                    printf("len  : %ubyte\n", packet.getLength());
                    printf("dst : %.12" PRIx64 "\n", packet.getDst().toLong());
                    printf("src : %.12" PRIx64 "\n", packet.getSrc().toLong());
                    dlib::hexdump(buf, s);
                    printf("\n");
                    */
                }
            }
        }
    }
    
    return(0);
}
