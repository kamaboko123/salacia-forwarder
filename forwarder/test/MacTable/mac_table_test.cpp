#include <stdio.h>
#include <unistd.h>
#include "MacTable.hpp"

int main(void){
    MacTable tbl(256);
    
    NetIf if1("11h", IFTYPE_L2_ACCESS, 1);
    PointerWrapper<NetIf *> _if1(&if1);
    
    MacAddress mac1("11:22:33:44:55:66");
    tbl.update(mac1, _if1);
    printf("%s\n", tbl.get(mac1).unwrap()->getIfName());
    tbl.refresh();
    
    sleep(11);
    
    tbl.refresh();
    
    if(tbl.get(mac1).unwrap() == nullptr){
        printf("[main]deleted\n");
    }
    return(0);
}
