#include <cstdio>
#include "IPAddress.hpp"
#include "comlib.hpp"

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
   /*
    if(mask.isValid()){
        printf("OK!\n");
    }
    else{
        printf("NG!\n");
    }*/
    
    printf("----\n");
    
    IPAddress network1_addr((char*)"192.168.0.0");
    IPNetmask network1_mask((char*)"255.255.252.0");
    
    /*
    //IPNetwork network1((char *)"10.1.2.0/24");
    IPNetwork network1(network1_addr, network1_mask);
    if(network1.isValid()){
        printf("OK!\n");
    }
    else{
        printf("NG!\n");
    }
    printf("%s\n", network1.toStr());
    */
    
    
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
    
    /*
    if(cp_nw2.isValid()){
        printf("OK!\n");
    }
    else{
        printf("NG!\n");
    }
    printf("%s\n", cp_nw2.toStr());
    */
    
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
    
    
    IPAddress *valid_addr;
    try{
        valid_addr = new IPAddress((char *)"255.25s.255.255");
        delete valid_addr;
    }
    catch(sfwdr::Exception::InvalidIPAddress &e){
        printf("%s\n", e.getMessage());
    }
    //printf("%p", valid_addr);
    //delete valid_addr;
    //printf("%s\n", valid_addr->toStr());
    valid_addr = new IPAddress((char *)"255.255.255.255");
    printf("%s\n", valid_addr->toStr());
    delete valid_addr;
    //IPAddress((char *)"999.999.999.999");
    
    IPNetmask *m;
    try{
        m = new IPNetmask("999.999.999.999");
        delete m;
    }
    catch(sfwdr::Exception::InvalidIPNetmask &e){
        printf("%s\n", e.getMessage());
    }
    
    try{
        m = new IPNetmask(0xffffff00);
        printf("!!%s\n", m->toStr());
        delete m;
    }
    catch(sfwdr::Exception::InvalidIPNetmask &e){
        printf("%s\n", e.getMessage());
    }
    
    try{
        m = new IPNetmask(0xfffff100);
        printf("!!!%s\n", m->toStr());
        delete m;
    }
    catch(sfwdr::Exception::InvalidIPNetmask &e){
        printf("%s\n", e.getMessage());
    }
    
    m = new IPNetmask();
    try{
        printf("!!!%s\n", m->toStr());
        m->setLength(23);
        printf("!!!%s\n", m->toStr());
        m->setLength(33);
        printf("!!!%s\n", m->toStr());
    }
    catch(sfwdr::Exception::InvalidIPNetmask &e){
        printf("%s\n", e.getMessage());
    }
    printf("!!!%s\n", m->toStr());
    delete m;
    
    m = new IPNetmask();
    try{
        printf("!!!%s\n", m->toStr());
        m->set("255.2.2.2");
    }
    catch(sfwdr::Exception::InvalidIPNetmask &e){
        printf("%s\n", e.getMessage());
    }
    printf("!!!%s\n", m->toStr());
    delete m;
    
    m = new IPNetmask();
    try{
        printf("!!!%s\n", m->toStr());
        m->set(0xFFFF1000);
    }
    catch(sfwdr::Exception::InvalidIPNetmask &e){
        printf("%s\n", e.getMessage());
    }
    printf("!!!%s\n", m->toStr());
    delete m;
    
    IPNetwork::validIPNetwork((char *)"10.0.0.0/8");
    try{
        IPNetwork::validIPNetwork((char *)"10.0.0.2222/9");
    }
    catch(sfwdr::Exception::Exception &e){
        printf("%s\n", e.getMessage());
    }
    
    try{
        IPNetwork::validIPNetwork((char *)"10.0.0.222");
    }
    catch(sfwdr::Exception::Exception &e){
        printf("%s\n", e.getMessage());
    }
    
    IPNetwork *net1;
    try{
        net1 = new IPNetwork((char *)"10.0.0.222");
        delete net1;
    }
    catch(sfwdr::Exception::Exception &e){
        printf("%s\n", e.getMessage());
    }
    
    net1 = new IPNetwork();
    delete net1;
    
    return(0);
}
