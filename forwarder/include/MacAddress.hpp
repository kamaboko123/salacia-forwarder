#ifndef INCLUDED_MAC_ADDRESS
#define INCLUDED_MAC_ADDRESS

#include <cstdint>
#include <iostream>

#define MAC_ADDR_SIZE 6

class MacAddress{
private:
    uint64_t addr;
    static const uint64_t hbase = 256;
public:
    MacAddress();
    MacAddress(const char *addr_str);
    MacAddress(const uint64_t addr_int);
    
    uint64_t toLong() const;
    uint64_t getHash() const;
    bool operator==(MacAddress v);
    bool operator!=(MacAddress v);
    MacAddress &operator=(const uint64_t addr);
};

#endif
