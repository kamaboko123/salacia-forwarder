#include <cstdio>
#include "../../src/RouteTable.hpp"

int main(void){
    
    //Route Test
    IPAddress addr1 = IPAddress("192.168.1.1");
    IPAddress addr2 = IPAddress("192.168.2.255");
    try{
        Route *r1 = new Route("10.0.0.0/16");
        r1->addNexthop(RTYPE_STATIC, addr1);
        r1->addNexthop(RTYPE_CONNECTED, addr2);
        
        Array<IPAddress> best1;
        r1->getBestNexthops(best1);
        printf("r1 best : %s\n", best1.get(0).toStr());
        
        Route r2 = *r1;
        
        delete r1;
    }
    catch(sfwdr::Exception::Exception e){
        printf("%s\n", e.getMessage());
    }
    
    //RouteTable Test
    IPNetwork nw2("10.10.10.0/24");
    RouteTable *rtb = new RouteTable();
    rtb->addRoute(nw2, RTYPE_STATIC, addr1);
    printf("%s\n", rtb->getRoute(nw2).getBestNexthops().get(0).toStr());
    
    
    delete rtb;
    
    return(0);
}
