#include <cstdio>
#include "../../src/IPAddress.hpp"
#include "../../src/comlib.hpp"

int main(void){
    IPAddress addr((char *)"192.168.1.1");
    
    printf("%u\n", addr.touInt());
    printf("%s\n", addr.toStr());
    
    //addr.set(3232235777);
    addr.set((uint32_t)0);
    printf("%s\n", addr.toStr());
    
    IPNetMask mask((char *)"255.255.255.0");
    printf("%s\n", mask.toStr());
    printf("%d\n", mask.getLength());
    if(mask.isValid()){
        printf("OK!\n");
    }
    else{
        printf("NG!\n");
    }
    
    printf("----\n");
    
    IPNetMask mask2;
    mask2.setLength(24);
    printf("%s\n", mask2.toStr());
    printf("%d\n", mask2.getLength());
    if(mask2.isValid()){
        printf("OK!\n");
    }
    else{
        printf("NG!\n");
    }
    
    
    return(0);
}
