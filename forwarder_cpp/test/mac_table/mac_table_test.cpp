#include <iostream>
#include "../../src/MacTable.hpp"

int main(void){
    MacTable mtbl;
    NetIf nif("veth6b1a875", IFTYPE_L2_ACCESS, 1);
    
    mtbl.addEntry(nif.getMacStr(), &nif);
    
    //std::cout << nif.getMacStr() << std::endl;
    
    std::cout << mtbl.getEntry(nif.getMacStr())->getNetIf()->getMacStr() << std::endl;
    std::cout << mtbl.getEntry(nif.getMacStr())->getTimeInt() << std::endl;
    
    return(0);
}

