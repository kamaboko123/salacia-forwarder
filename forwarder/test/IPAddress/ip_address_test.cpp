#include <cstdio>
#include "../../src/IPAddress.hpp"
#include "../../src/comlib.hpp"

int main(void){
    IPAddress addr((char *)"172.18.202.101");
    
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
    
    
    
    printf("\n\n\n[copy test]\n");
    
    IPAddress cp_addr1((char *)"10.0.0.0");
    IPAddress cp_addr2;
    cp_addr2 = cp_addr1;
    IPAddress cp_addr3 = cp_addr2;
    
    
    IPNetmask cp_mask1((char *)"255.255.255.0");
    IPNetmask cp_mask2;
    cp_mask2 = cp_mask1;
    IPNetmask cp_mask3 = cp_mask2;
    
    
    IPNetwork cp_nw1((char *)"10.0.0.0/8");
    IPNetwork cp_nw2 = cp_nw1;
    //cp_nw2.set("0.0.0.0/1");
    cp_nw2 = cp_nw1;
    IPNetwork cp_nw3 = cp_nw2;
    
    if(cp_nw2.isValid()){
        printf("OK!\n");
    }
    else{
        printf("NG!\n");
    }
    printf("%s\n", cp_nw2.toStr());
    
    printf("[IPAddress set_flg test]\n");
    IPAddress default_addr;
    if(default_addr.isSet()){
        printf("Set:");
    }
    else{
        printf("not Set:");
    }
    printf("%s\n", default_addr.toStr());
    
    IPAddress copied_default_addr = default_addr;
    if(copied_default_addr.isSet()){
        printf("Set:");
    }
    else{
        printf("not Set:");
    }
    printf("%s\n", copied_default_addr.toStr());
    
    printf("[IPNetmask set_flg test]\n");
    IPNetmask default_mask;
    if(default_mask.isSet()){
        printf("Set:");
    }
    else{
        printf("not Set:");
    }
    printf("%s\n", default_mask.toStr());
    
    return(0);
}
