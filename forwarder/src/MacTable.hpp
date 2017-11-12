#ifndef INCLUDED_MAC_TABLE
#define INCLUDED_MAC_TABLE

#include <ctime>
#include "MacAddress.hpp"
#include "HashMap.hpp"
#include "NetIf.hpp"

#define MAC_TBL_EXPIRE_TIME 5

class MacTableEntry{
private:
    NetIf *netif;
    uint64_t last_ref_time;
    void update_time();
    
public:
    MacTableEntry(NetIf *interface);
    void update(NetIf *interface);
    NetIf *getIf();
    uint64_t getRefTime();
    void updateRefTime();
};

class MacTable{
private:
    HashMap<MacAddress, MacTableEntry *> *tbl;

public:
    MacTable(int size = 256);
    ~MacTable();
    void update(MacAddress addr, NetIf *interface);
    NetIf *get(MacAddress addr);
    void refresh();
    
    MacAddress *getKeys();
    int getSize();
};

#endif
