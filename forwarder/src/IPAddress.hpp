#include <cstdint>
#include <cstdio>
#include "comlib.hpp"

#define IP_ADDR_STR_LEN 16

class IPAddress{
private:
    uint32_t addr;
    char *addr_str;
    
    void _init();
    
public:
    IPAddress();
    IPAddress(uint32_t addr_uint);
    IPAddress(char *addr_str);
    void set(uint32_t addr_uint);
    void set(char *addr_str);
    
    uint32_t touInt();
    char *toStr();
    
    static uint32_t iptoui(char *addr_str);
    static char *uitoip(uint32_t addr, char *retbuf, sfwdr::size_t retbuf_len);
};
