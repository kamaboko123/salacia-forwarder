#include <cstdint>
#include <cstdio>
#include "comlib.hpp"

//IPAddressとIPNetMaskを文字列にしたときに必要なバッファ
#define IP_ADDR_STR_LEN 16
#define IP_PREFIX_STR_LEN 19

//IPNetMaskをinvalidにする時の定数
#define IP_NETMASK_INVALID_VAL 3

//IPNetowrkをinvalidにする時の定数
#define IP_NETWORK_INVALID_NWADDR 1
#define IP_NETWORK_INVALID_MASK 0

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
    //prefix形式の文字列 (例:192.168.0.0/24)
    IPNetwork(char *ipnet_str);
    //ネットワークアドレスは文字列、マスクはマスク長
    IPNetwork(char *addr_str, sfwdr::size_t mask_length);
    //ネットワークアドレスは文字列、マスクはマスク長
    IPNetwork(IPAddress &ipaddr, sfwdr::size_t mask_length);
    //ネットワークアドレスとマスクのオブジェクト
    IPNetwork(IPAddress &ipaddr, IPNetMask &netmask);
    
    ~IPNetwork();
    
    IPAddress *getNetAddr();
    IPAddress *getNetMask();
    
    char *toStr();
    
    bool isValid();
    
    static bool validPrefixFormat(char *str);
};

