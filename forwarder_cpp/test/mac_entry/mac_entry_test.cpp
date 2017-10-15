#include <iostream>
#include "../../src/MacEntry.hpp"

int main(void){
    
    NetIf nif("enp4s0", IFTYPE_L2_ACCESS, 1);
    MacEntry entry(&nif);
    std::cout << entry.getNetIf()->getMacStr() << std::endl;
    
    return(0);
}

