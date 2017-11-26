#include <cstdio>
#include "../../src/RouteTable.hpp"

int main(void){
    
    //Route Test
    IPAddress addr1 = IPAddress("192.168.1.1");
    IPAddress addr2 = IPAddress("192.168.2.254");
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
    IPNetwork nw2("10.10.0.0/16");
    IPNetwork nw3("192.168.2.0/24");
    IPNetwork nw_default("0.0.0.0/0");
    IPNetwork nw_connected1("192.168.1.0/24");
    
    IPAddress addr_default("0.0.0.0");
    RouteTable *rtb = new RouteTable();
    rtb->addRoute(nw2, RTYPE_STATIC, addr2);
    rtb->addRoute(nw3, RTYPE_STATIC, addr1);
    rtb->addRoute(nw_default, RTYPE_STATIC, addr1);
    rtb->addRoute(nw_connected1, RTYPE_CONNECTED, addr_default);
    //printf("%s\n", rtb->getRoute(nw2).getBestNexthops().get(0).toStr());
    
    
    //IPNetwork nw3("10.10.10.0/24");
    //rtb->addRoute(nw3, RTYPE_STATIC, addr2);
    //IPAddress addr3("10.10.10.1");
    IPAddress addr3("10.10.10.1");
    //printf("%s\n", rtb->resolve(addr3).getNetwork().toStr());
    printf("%s\n", rtb->r_resolve(addr3).getNetwork().toStr());
    
    //Route test((char *)"0.0.0.0/0");
    
    delete rtb;
    
    return(0);
}
