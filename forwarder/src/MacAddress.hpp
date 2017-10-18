#ifndef INCLUDED_MAC_ADDRESS
#define INCLUDED_MAC_ADDRESS

#include <cstdint>
#include <iostream>

class MacAddress{
private:
    uint64_t addr;
    const uint64_t hbase = 256;
public:
    MacAddress(char *addr_str);
    MacAddress(uint64_t addr_int);
    uint64_t toInt();
    uint64_t getHash();
};

#endif
