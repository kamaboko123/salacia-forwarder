#include <iostream>
#include <unordered_map>
#include "MacEntry.hpp"

using namespace std;

class MacTable{
private:
    unordered_map<string, MacEntry *> tbl;

public:
    //MacTable();
    void addEntry(string addr, NetIf *nif);
    MacEntry *getEntry(string addr);
    NetIf *getNetIf(string addr);
};
