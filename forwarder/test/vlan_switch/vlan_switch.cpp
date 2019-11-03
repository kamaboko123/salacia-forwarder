#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <poll.h>
#include <unistd.h>
#include <cinttypes>
#include <new>

#include "NetIf.hpp"
#include "MacAddress.hpp"
#include "Ethernet.hpp"
#include "HashMap.hpp"
#include "MacTable.hpp"
#include "Array.hpp"
#include "comlib.hpp"
#include "dlib.hpp"

#define IF_NUM 3
#define MAC_REFRESH_INTERVAL 10

class Vlan{
public:
    uint16_t id;
    MacTable mtbl;
    Array<NetIf *> ifs;
};

template<>
int HashMap<uint16_t, Vlan *>::hash(uint16_t key) const{
    return(key % this->size);
}

bool exit_flg = false;

struct IF_CONFIG{
    char name[IFNAMSIZ];
    IfType type;
    uint16_t vlan;
};

void handler_sigint(int signal){
    if(signal == SIGINT){
        printf("receive SIGINT\n");
        exit_flg = true;
    }
}

int main(int argc, char **argv){
    int inter_n = IF_NUM;
    
    IF_CONFIG ifc[IF_NUM] = {
        {
            "enp2s0",
            IFTYPE_L2_ACCESS,
            3200
        },
        {
            "enp3s0",
            IFTYPE_L2_ACCESS,
            3100
        },
        {
            "enp4s0",
            IFTYPE_L2_TRUNK,
            0
        }
    };
    
    //descriptor for polling
    struct pollfd *pfds = new struct pollfd[inter_n];
    NetIf *netif = new NetIf[inter_n];
    
    HashMap<uint16_t, Vlan *> *vlan_db = new HashMap<uint16_t, Vlan *>(256);
    
    for(int i = 0; i < inter_n; i++){
        new(netif + i) NetIf(ifc[i].name, ifc[i].type, ifc[i].vlan);
        printf("set : %s(%d)\n", netif[i].getIfName(), ifc[i].type);
        pfds[i].fd = netif[i].getFD();
        pfds[i].events = POLLIN|POLLERR;
        
        if(ifc[i].type == IFTYPE_L2_ACCESS){
            Vlan *vlan = vlan_db->get(ifc[i].vlan);
            if(vlan == nullptr){
                printf("!%s!\n", netif[i].getIfName());
                Vlan *new_vlan = new Vlan();
                new_vlan->id = ifc[i].vlan;
                new_vlan->ifs.add(&netif[i]);
                vlan_db->update(ifc[i].vlan, new_vlan);
                continue;
            }
            vlan->ifs.add(&netif[i]);
        }
    }
    
    for(int i = 0; i < inter_n; i++){
        if(netif[i].getIfType() == IFTYPE_L2_TRUNK){
            uint16_t *vlans = vlan_db->getKeys();
            for(int j = 0; j < vlan_db->getSize(); j++){
                vlan_db->get(vlans[j])->ifs.add(&netif[i]);
            }
        }
    }
    
    uint16_t *vlans;
    vlans = vlan_db->getKeys();
    for(int i = 0; i < vlan_db->getSize(); i++){
        printf("[%u]\n", vlans[i]);
        for(uint32_t j = 0; j < vlan_db->get(vlans[i])->ifs.getSize(); j++){
            printf("  %s\n", vlan_db->get(vlans[i])->ifs.get(j)->getIfName());
        }
    }
    delete[] vlans;
    
    //receive buffer
    Ethernet pbuf;
    
    NetIf *outif;
    Vlan *vlan;
    
    signal(SIGINT, handler_sigint);
    
    uint64_t last_refresh = time(NULL);
    
    for(;;){
        if(exit_flg) break;
        
        if((time(NULL) - last_refresh) > MAC_REFRESH_INTERVAL){
            vlans = vlan_db->getKeys();
            for(int l = 0; l < vlan_db->getSize(); l++){
                printf("refresh : mac-table[vlan : %d]\n", vlans[l]);
                vlan_db->get(vlans[l])->mtbl.refresh();
                last_refresh = time(NULL);
            }
        }
        
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
                    if(pbuf.getType() == ETHTYPE_UNKNOWN) continue;
                    
                    if(netif[i].getIfType() == IFTYPE_L2_TRUNK){
                        vlan = vlan_db->get(pbuf.getVlanId());
                    }
                    else if(netif[i].getIfType() == IFTYPE_L2_ACCESS){
                        vlan = vlan_db->get(netif[i].getVlanId());
                    }
                    
                    if(vlan == nullptr){
                        printf("error : vlan not found!\n");
                        continue;
                    }
                    
                    vlan->mtbl.update(pbuf.getSrc(), PointerWrapper<NetIf>(&(netif[i])));
                    outif = vlan->mtbl.get(pbuf.getDst()).unwrap();
                    
                    if((pbuf.getDst().toLong() == 0xFFFFFFFFFFFF) || (outif == nullptr)){
                        for(uint32_t j = 0; j < vlan->ifs.getSize(); j++){
                            if(&netif[i] == vlan->ifs.get(j)) continue;
                            vlan->ifs.get(j)->send(pbuf, vlan->id);
                        }
                        continue;
                    }
                    outif->send(pbuf, vlan->id);
                }
            }
        }
    }
    
    delete[] netif;
    delete[] pfds;
    
    
    printf("Exit\n");
    return(0);
}
