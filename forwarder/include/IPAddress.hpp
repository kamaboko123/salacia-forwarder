#ifndef INCLUDED_IPADDRESS
#define INCLUDED_IPADDRESS

#include <cstdint>
#include <cstdio>
#include "comlib.hpp"
#include "Exception.hpp"

#define IP_ADDR_SIZE 4

//IPAddressとIPNetmaskを文字列にしたときに必要なバッファ
#define IP_ADDR_STR_LEN 16
#define IP_PREFIX_STR_LEN 20

class IPAddress{
private:
    uint32_t addr;
    char *addr_str;
    
    void _init();
    void _free();
    
public:
    IPAddress();
    IPAddress(uint32_t addr_uint);
    IPAddress(const char *addr_str);
    
    ~IPAddress();
    
    //コピーコンストラクタと代入演算子
    IPAddress(const IPAddress &ipaddr);
    IPAddress &operator=(const IPAddress &ipaddr);
    
    void set(uint32_t addr_uint);
    void set(const char *addr_str);
    void set(const IPAddress &ipaddr);
    
    uint32_t touInt() const;
    char *toStr() const;
    
    static uint32_t iptoui(const char *addr_str);
    static char *uitoip(uint32_t addr, char *retbuf, sfwdr::ssize_t retbuf_len);
    
    bool operator==(IPAddress v);
};

class IPNetmask : public IPAddress{
private:
    sfwdr::ssize_t length;
    
public:
    IPNetmask();
    IPNetmask(const char *addr_str);
    IPNetmask(uint32_t addr_uint);
    
    sfwdr::ssize_t set(uint32_t addr_uint);
    sfwdr::ssize_t set(const char *addr_str);
    sfwdr::ssize_t setLength(sfwdr::ssize_t mask_length);
    
    sfwdr::ssize_t getLength() const;
    
    static sfwdr::size_t validIPNetmask(uint32_t mask);
};

typedef struct{
    IPAddress netaddr;
    IPNetmask netmask;
} IPNW;

class IPNetwork{
private:
    IPNW *ipnw;
    
    char *prefix;
    
    void _init();
    void _free();
    void _build_str();
    
public:
    IPNetwork();
    //prefix形式の文字列 (例:192.168.0.0/24)
    IPNetwork(const char *ipnet_str);
    //ネットワークアドレスはオブジェクト、マスクはマスク長
    IPNetwork(const IPAddress &ipaddr, sfwdr::ssize_t mask_length);
    
    ~IPNetwork();
    
    IPNetwork(const IPNetwork &ipnet);
    IPNetwork &operator=(const IPNetwork &ipnet);
    
    const IPAddress &getNetaddr() const;
    const IPNetmask &getNetmask() const;
    
    void set(const IPNetwork &ipnet);
    void set(const char *nw_str);
    void set(const IPAddress &ipaddr, sfwdr::ssize_t mask_length);
    
    const char *toStr() const;
    
    static IPNW validIPNetwork(const char *nw_str);
    static IPNW validIPNetwork(const IPAddress &ipaddr, sfwdr::ssize_t mask_length);
};

#endif
