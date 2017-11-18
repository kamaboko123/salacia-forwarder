#include <cstdint>
#include <cstdio>
#include "comlib.hpp"

#define IP_ADDR_STR_LEN 16
#define IP_NETMASK_INVALID_VAL 3
#define IP_PREFIX_LEN 19

class IPAddress{
private:
    uint32_t addr;
    char *addr_str;
    
    void _init();
    
public:
    IPAddress();
    IPAddress(uint32_t addr_uint);
    IPAddress(char *addr_str);
    
    ~IPAddress();
    
    void set(uint32_t addr_uint);
    void set(char *addr_str);
    void set(IPAddress ipaddr);
    
    uint32_t touInt();
    char *toStr();
    
    static uint32_t iptoui(char *addr_str);
    static char *uitoip(uint32_t addr, char *retbuf, sfwdr::size_t retbuf_len);
};

class IPNetMask : public IPAddress{
private:
    bool valid;
    sfwdr::size_t length;
    
    bool _validate();
public:
    IPNetMask();
    IPNetMask(char *addr_str);
    IPNetMask(uint32_t addr_uint);
    
    sfwdr::size_t set(uint32_t addr_uint);
    sfwdr::size_t set(char *addr_str);
    sfwdr::size_t setLength(sfwdr::size_t mask_length);
    
    bool isValid();
    sfwdr::size_t getLength();
    
};

class IPNetwork{
private:
    bool valid;
    IPAddress *netaddr;
    IPNetMask *netmask;
    char *prefix;
    
    void _init();
    bool _validate();
    
public:
    IPNetwork(char *addr_str, sfwdr::size_t netmask);
    IPNetwork(IPAddress &ipaddr, sfwdr::size_t netmask);
    
    IPAddress *getNetAddr();
    IPAddress *getNetMask();
    
    char *toStr();
    
    bool isValid();
};

