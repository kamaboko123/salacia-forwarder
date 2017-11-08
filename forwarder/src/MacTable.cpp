#include "MacTable.hpp"

MacTableEntry::MacTableEntry(NetIf *interface){
    this->netif = interface;
    updateRefTime();
}

void MacTableEntry::updateRefTime(){
    last_ref_time = time(NULL);
}

uint64_t MacTableEntry::getRefTime(){
    return(last_ref_time);
}

void MacTableEntry::update(NetIf *interface){
    this->netif = interface;
    updateRefTime();
}

NetIf *MacTableEntry::getIf(){
    updateRefTime();
    return(netif);
}



MacTable::MacTable(int size){
    this->tbl = new HashMap<MacAddress, MacTableEntry *>(size);
}

MacTable::~MacTable(){
    delete tbl;
}

void MacTable::update(MacAddress addr, NetIf *interface){
    MacTableEntry *entry = tbl->get(addr);
    if(entry == nullptr){
        MacTableEntry *new_entry = new MacTableEntry(interface);
        tbl->update(addr, new_entry);
        return;
    }
    
    entry->update(interface);
}

NetIf *MacTable::get(MacAddress addr){
    MacTableEntry *entry = tbl->get(addr);
    if(entry == nullptr) return(nullptr);
    return(entry->getIf());
}

void MacTable::refresh(){
    uint64_t c_time = time(NULL);
    int size = tbl->getSize();
    
    MacAddress *keys = new MacAddress[size];
    tbl->getKeys(keys);
    for(int i = 0; i < size; i++){
        MacTableEntry *entry = tbl->get(keys[i]);
        if((c_time - entry->getRefTime()) > MAC_TBL_EXPIRE_TIME){
            tbl->del(keys[i]);
            delete entry;
        }
    }
    
    delete[] keys;
    
}
