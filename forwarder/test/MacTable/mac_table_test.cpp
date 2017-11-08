#include <stdio.h>
#include <unistd.h>
#include "../../src/MacTable.hpp"

int main(void){
    MacTable tbl(256);
    
    NetIf if1("enp4s0", IFTYPE_L2_ACCESS, 1);
    MacAddress mac1("11:22:33:44:55:66");
    tbl.update(mac1, &if1);
    printf("%s\n", tbl.get(mac1)->getIfName());
    
    tbl.refresh();
    
    sleep(10);
    
    tbl.refresh();
    
    if(tbl.get(mac1) == NULL){
        printf("[main]deleted\n");
    }
    return(0);
}
