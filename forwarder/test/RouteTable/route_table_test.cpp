#include <cstdio>
#include "../../src/RouteTable.hpp"

int main(void){
    RouteTable rtbl;
    
    IPNetwork nw1((char *)"192.168.100.0/24");
    Route r1((char *)"10.0.0.0/24");
    IPAddress nh1_1((char *)"192.168.0.1");
    
    r1.addNexthop(RTYPE_STATIC, nh1_1);
    Array<IPAddress> *nhs1 = r1.getNexthops(RTYPE_STATIC);
    printf("%s\n", nhs1->get(0).toStr());
    
    IPNetwork nw2((char *)"172.16.100.0/24");
    IPAddress nh2_1((char *)"192.168.1.1");
    rtbl.addRoute(nw2, RTYPE_STATIC, nh2_1);
    Route *r2 = rtbl.getRoute(nw2);
    
    if(r2 != nullptr){
        Array<IPAddress> *nh = r2->getNexthops(RTYPE_STATIC);
        if(nh != nullptr){
            printf("Number of Nexthops : %d\n", nh->getSize());
            for(uint32_t i = 0; i < nh->getSize(); i++){
                printf("[%d]%s\n", i + 1, nh->get(i).toStr());
            }
        }
    }
    return(0);
}
