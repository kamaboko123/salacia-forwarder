#include <cstdio>
#include "../../src/IPAddress.hpp"
#include "../../src/comlib.hpp"

int main(void){
    IPAddress addr((char *)"192.168.1.1");
    
    printf("%u\n", addr.touInt());
    printf("%s\n", addr.toStr());
    
    addr.set(3232235777);
    printf("%s\n", addr.toStr());
    
    return(0);
}
