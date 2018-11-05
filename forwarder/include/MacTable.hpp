#ifndef INCLUDED_MAC_TABLE
#define INCLUDED_MAC_TABLE

#include <ctime>
#include "MacAddress.hpp"
#include "HashMap.hpp"
#include "NetIf.hpp"

#define MAC_TBL_EXPIRE_TIME 5

class MacTableItem{
private:
    NetIf *netif;
    uint64_t last_ref_time;
    void update_time();
    
public:
    MacTableItem(NetIf *interface);
    void update(NetIf *interface);
    NetIf *getIf();
    uint64_t getRefTime();
    void updateRefTime();
};

class MacTable{
private:
    HashMap<MacAddress, MacTableItem *> *tbl;
    Array<MacAddress> tbl_keys;

public:
    MacTable(int size = 256);
    ~MacTable();
    void update(MacAddress addr, NetIf *interface);
    NetIf *get(MacAddress addr);
    void refresh();
    
    int getSize();
};

#endif
