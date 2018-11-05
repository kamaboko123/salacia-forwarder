#include "MacTable.hpp"

MacTableItem::MacTableItem(NetIf *interface){
    this->netif = interface;
    updateRefTime();
}

void MacTableItem::updateRefTime(){
    last_ref_time = time(NULL);
}

uint64_t MacTableItem::getRefTime(){
    return(last_ref_time);
}

void MacTableItem::update(NetIf *interface){
    this->netif = interface;
    updateRefTime();
}

NetIf *MacTableItem::getIf(){
    updateRefTime();
    return(netif);
}



MacTable::MacTable(int size){
    this->tbl = new HashMap<MacAddress, MacTableItem *>(size);
}

MacTable::~MacTable(){
    delete tbl;
}

void MacTable::update(MacAddress addr, NetIf *interface){
    MacTableItem *entry = tbl->get(addr);
    if(entry == nullptr){
        MacTableItem *new_entry = new MacTableItem(interface);
        tbl->update(addr, new_entry);
        return;
    }
    
    entry->update(interface);
}

NetIf *MacTable::get(MacAddress addr){
    MacTableItem *entry = tbl->get(addr);
    if(entry == nullptr) return(nullptr);
    return(entry->getIf());
}

void MacTable::refresh(){
    uint64_t c_time = time(NULL);
    Array<MacAddress> *keys = new Array<MacAddress>;
    tbl->getKeys(*keys);
    for(sfwdr::size_t i = 0; i < keys->getSize(); i++){
        MacTableItem *entry = tbl->get(keys->get(i));
        if((c_time - entry->getRefTime()) > MAC_TBL_EXPIRE_TIME){
            tbl->del(keys->get(i));
            delete entry;
        }
    }
    
    delete keys;
}

int MacTable::getSize(){
    return(tbl->getSize());
}

