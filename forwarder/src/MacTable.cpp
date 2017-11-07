#include "MacTable.hpp"

MacTableEntry::MacTableEntry(NetIf *interface){
    this->netif = interface;
}

NetIf *MacTableEntry::getIf(){
    return(netif);
}

MacTable::MacTable(int size){
    this->tbl = new HashMap<MacAddress, MacTableEntry *>(size);
}

