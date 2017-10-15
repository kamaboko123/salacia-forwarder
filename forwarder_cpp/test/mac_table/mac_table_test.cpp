#include <iostream>
#include "../../src/MacTable.hpp"

int main(void){
    MacTable mtbl;
    NetIf nif("enp4s0", IFTYPE_L2_ACCESS, 1);
    //MacEntry entry(&nif);
    
    mtbl.addEntry(nif.getMacStr(), &nif);
    
    std::cout << mtbl.getEntry("00:3d:2c:15:24:57")->getNetIf()->getMacStr() << std::endl;
    std::cout << mtbl.getEntry("00:3d:2c:15:24:57")->getTimeInt() << std::endl;
    
    return(0);
}

