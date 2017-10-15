#ifndef INCLUDED_MAC_ENTRY
#define INCLUDED_MAC_ENTRY

#include <iostream>
#include <ctime>
#include "NetIf.hpp"

using namespace std;

class MacEntry{
private:
    NetIf *pif;
    uint64_t last_acc_t;
    
public:
    MacEntry(NetIf *pif);
    void updateTime();
    NetIf *getNetIf();
    uint64_t getTimeInt();
};

#endif
