#ifndef INCLUDED_MAC_TABLE
#define INCLUDED_MAC_TABLE

#include "MacAddress.hpp"
#include "HashMap.hpp"
#include "NetIf.hpp"

class MacTableEntry{
private:
    NetIf *netif;
    //uint64_t last_ref_time;
    
public:
    MacTableEntry(NetIf *interface);
    //void update(NetIf *interface);
    NetIf *getIf();
};

class MacTable{
private:
    HashMap<MacAddress, MacTableEntry *> *tbl;

public:
    MacTable(int size);
    
};

#endif
