#include <cstdio>
#include "../../src/RouteTable.hpp"

int main(void){
    //RouteTable rtb();
    
    Route r1((char *)"10.0.0.0/24");
    IPAddress nh1_1((char *)"192.168.0.1");
    r1.addNexthop(RTYPE_STATIC, nh1_1);
    
    Array<IPAddress *> *nhs1 = r1.getNexthops(RTYPE_STATIC);
    printf("%s\n", nhs1->get(0)->toStr());
    
    return(0);
}
