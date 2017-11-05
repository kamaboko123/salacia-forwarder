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

#define IFMAX 4

int main(int argc, char **argv){
    int inter_n = 0;
    
    //perser
    Ethernet packet;
    
    
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
        new(netif + i) NetIf(argv[i + 1], IFTYPE_L2_ACCESS, 1);
        pfds[i].fd = netif[i].getFD();
        pfds[i].events = POLLIN|POLLERR;
        
    }
    
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
                    if((s = read(netif[i].getFD(), buf, sizeof(buf))) <= 0){
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
                                netif[j].sendRaw(buf, s);
                            }
                           continue;
                        }
                        
                        outif->sendRaw(buf, s);
                        
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
