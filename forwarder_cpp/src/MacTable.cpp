#include "MacTable.hpp"

void MacTable::addEntry(string addr, NetIf *nif){
    this->tbl[addr] = new MacEntry(nif);
}

MacEntry *MacTable::getEntry(string addr){
    return(this->tbl[addr]);
}

NetIf *MacTable::getNetIf(string addr){
    return(this->tbl[addr]->getNetIf());
}
