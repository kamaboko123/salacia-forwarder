#ifndef INCLUDED_IPADDRESS
#define INCLUDED_IPADDRESS

#include <cstdint>
#include <cstdio>
#include "comlib.hpp"

//IPAddressとIPNetmaskを文字列にしたときに必要なバッファ
#define IP_ADDR_STR_LEN 16
#define IP_PREFIX_STR_LEN 20

//IPNetmaskをinvalidにする時の定数
#define IP_NETMASK_INVALID_VAL 3

//IPNetowrkをinvalidにする時の定数
#define IP_NETWORK_INVALID_NWADDR 1
#define IP_NETWORK_INVALID_MASK 0

class IPAddress{
private:
    uint32_t addr;
    char *addr_str;
    
    void _init();

protected:
    bool set_flg;
    
public:
    IPAddress();
    IPAddress(uint32_t addr_uint);
    IPAddress(char *addr_str);
    
    ~IPAddress();
    
    //コピーコンストラクタと代入演算子
    IPAddress(const IPAddress &ipaddr);
    IPAddress &operator=(const IPAddress &ipaddr);
    
    void set(uint32_t addr_uint, bool set_flg = true);
    void set(char *addr_str, bool set_flg = true);
    void set(const IPAddress &ipaddr);
    
    uint32_t touInt() const;
    char *toStr() const;
    bool isSet() const;
    
    static uint32_t iptoui(char *addr_str);
    static char *uitoip(uint32_t addr, char *retbuf, sfwdr::ssize_t retbuf_len);
};

class IPNetmask : public IPAddress{
private:
    bool valid;
    sfwdr::ssize_t length;
    
    bool _validate();
public:
    IPNetmask();
    IPNetmask(char *addr_str);
    IPNetmask(uint32_t addr_uint);
    
    sfwdr::ssize_t set(uint32_t addr_uint);
    sfwdr::ssize_t set(char *addr_str);
    sfwdr::ssize_t setLength(sfwdr::ssize_t mask_length);
    
    bool isValid() const;
    sfwdr::ssize_t getLength() const;
    
};

class IPNetwork{
private:
    bool valid;
    IPAddress *netaddr;
    IPNetmask *netmask;
    char *prefix;
    
    void _init();
    bool _validate();
    
public:
    IPNetwork();
    //prefix形式の文字列 (例:192.168.0.0/24)
    IPNetwork(char *ipnet_str);
    //ネットワークアドレスは文字列、マスクはマスク長
    IPNetwork(char *addr_str, sfwdr::ssize_t mask_length);
    //ネットワークアドレスはオブジェクト、マスクはマスク長
    IPNetwork(const IPAddress &ipaddr, sfwdr::ssize_t mask_length);
    //ネットワークアドレスとマスクのオブジェクト
    IPNetwork(const IPAddress &ipaddr, const IPNetmask &netmask);
    
    ~IPNetwork();
    
    IPNetwork(const IPNetwork &ipnet);
    IPNetwork &operator=(const IPNetwork &ipnet);
    
    IPAddress &getNetaddr() const;
    IPNetmask &getNetmask() const;
    
    bool set(const IPNetwork &ipnet);
    bool set(char *ipnet_str);
    bool set(const IPAddress &ipaddr, const IPNetmask &netmask);
    
    char *toStr() const;
    
    bool isValid() const;
    
    static bool validPrefixFormat(char *str);
};

#endif
