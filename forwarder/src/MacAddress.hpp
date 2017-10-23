#ifndef INCLUDED_MAC_ADDRESS
#define INCLUDED_MAC_ADDRESS

#include <cstdint>

class MacAddress{
private:
    uint64_t addr;
    static const uint64_t hbase = 256;
public:
    MacAddress();
    MacAddress(char *addr_str);
    MacAddress(uint64_t addr_int);
    
    uint64_t toInt();
    uint64_t toLong();
    uint64_t getHash();
    bool operator==(MacAddress v);
    bool operator!=(MacAddress v);
};

#endif
