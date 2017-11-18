#include <cstdio>
#include "../../src/IPAddress.hpp"
#include "../../src/comlib.hpp"

int main(void){
    IPAddress addr((char *)"192.168.1.1");
    
    printf("%u\n", addr.touInt());
    printf("%s\n", addr.toStr());
    
    //addr.set(3232235777);
    //addr.set((uint32_t)0);
    printf("%s\n", addr.toStr());
    
    IPNetmask mask((char *)"255.255.255.0");
    printf("%s\n", mask.toStr());
    printf("%d\n", mask.getLength());
    if(mask.isValid()){
        printf("OK!\n");
    }
    else{
        printf("NG!\n");
    }
    
    printf("----\n");
    
    IPNetmask mask2(16);
    printf("%s\n", mask2.toStr());
    printf("%d\n", mask2.getLength());
    if(mask2.isValid()){
        printf("OK!\n");
    }
    else{
        printf("NG!\n");
    }
    
    printf("----\n");
    
    IPAddress network1_addr((char*)"192.168.0.0");
    IPNetmask network1_mask((char*)"255.255.252.0");

    //IPNetwork network1((char *)"10.1.2.0/24");
    IPNetwork network1(network1_addr, network1_mask);
    if(network1.isValid()){
        printf("OK!\n");
    }
    else{
        printf("NG!\n");
    }
    printf("%s\n", network1.toStr());
    
    return(0);
}
