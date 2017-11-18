#include "IPAddress.hpp"


void IPAddress::_init(){
    addr = 0;
    addr_str = new char[IP_ADDR_STR_LEN];
    comlib::memset((uint8_t *)addr_str, 0, IP_ADDR_STR_LEN);
}

IPAddress::IPAddress(){
    _init();
    set((uint32_t)0);
}

IPAddress::IPAddress(uint32_t addr_uint){
    _init();
    set(addr_uint);
}

IPAddress::IPAddress(char *addr_str){
    _init();
    set(addr_str);
}

IPAddress::~IPAddress(){
    delete[] addr_str;
}

void IPAddress::set(uint32_t addr_uint){
    addr = addr_uint;
    uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);
}

void IPAddress::set(char *addr_str){
    addr = iptoui(addr_str);
    uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);
}

void IPAddress::set(IPAddress ipaddr){
    set(ipaddr.touInt());
}

uint32_t IPAddress::touInt(){
    return(addr);
}

char *IPAddress::toStr(){
    return(addr_str);
}

char *IPAddress::uitoip(uint32_t addr, char *retbuf, sfwdr::size_t retbuf_len){
    if(retbuf_len < IP_ADDR_STR_LEN) return(nullptr);
    
    uint32_t ext_base = 0xFF000000;
    uint8_t ls = 24;
    size_t i = 0;
    char buf;
    
    //上位から1byteずつ抜き出して処理
    while(ext_base != 0){
        
        //100の位(0の場合は入れない)
        buf = (((addr & ext_base) >> ls) / 100) + '0';
        if(buf != '0') retbuf[i++] = buf;
        
        //10の位(0の場合は入れない)
        buf = (((addr & ext_base) >> ls) % 100 / 10) + '0';
        if(buf != '0') retbuf[i++] = buf;
        
        //1の位(0でも入れる)
        retbuf[i++] = (((addr & ext_base) >> ls) % 10)+ '0';
        
        //ドット入れる
        retbuf[i++] = '.';
        
        ext_base = ext_base >> 8;
        ls -= 8;
    }
    
    //最後のドットは不要なのでそのまま終端
    retbuf[--i] = '\0';
    
    return(retbuf);
}

uint32_t IPAddress::iptoui(char *addr_str){
    uint32_t addr = 0;
    
    while(*addr_str != '\0'){
        //1byte桁上げ + 桁の値加算
        addr = (addr << 8) + comlib::atoi(addr_str);
        
        //次のオクテットまでポインタを進める
        do{
            addr_str++;
            if(*addr_str == '\0') break;
        }while(*(addr_str - 1) != '.');
    }
    //printf("%u\n", addr);
    
    return(addr);
}

IPNetMask::IPNetMask() : IPAddress(){
    _validate();
}

IPNetMask::IPNetMask(char *addr_str) : IPAddress(addr_str){
    _validate();
}

IPNetMask::IPNetMask(uint32_t addr_uint) : IPAddress(addr_uint){
    _validate();
}

sfwdr::size_t IPNetMask::set(uint32_t addr_uint){
    IPAddress::set(addr_uint);
    _validate();
    return(getLength());
}

sfwdr::size_t IPNetMask::set(char *addr_str){
    IPAddress::set(addr_str);
    _validate();
    return(getLength());
}

sfwdr::size_t IPNetMask::setLength(sfwdr::size_t mask_length){
    if(mask_length == 0) return(set((uint32_t)0));
    if(mask_length < 0 || mask_length > 32){
        set(IP_NETMASK_INVALID_VAL);
    }
    
    uint32_t mask = 0;
    uint8_t shift;
    for(int i = mask_length; i > 0; i--){
        shift = i + 31 - mask_length;
        mask += (1 << shift);
    }
    set(mask);
    
    return(getLength());
}

bool IPNetMask::_validate(){
    valid = true;
    length = 0;
    
    uint32_t mask = IPAddress::touInt();
    uint32_t cmask = 0x80000000;
    uint8_t buf;
    bool find_0 = false;
    
    for(int i = 32; i > 0 && valid; i--){
        buf = (mask & cmask) >> (i-1);
        
        if(find_0 && buf == 1) valid = false;
        if(buf == 0) find_0 = true;
        if(!find_0) length++;
        
        cmask = cmask >> 1;
    }
    if(!valid) length = -1;
    
    return(valid);
}

bool IPNetMask::isValid(){
    return(valid);
}

sfwdr::size_t IPNetMask::getLength(){
    return(length);
}

IPNetwork::IPNetwork(char *addr_str, sfwdr::size_t mask_length){
    _init();
    IPAddress ipaddr(addr_str);
    this->netaddr->set(ipaddr.touInt());
    this->netmask->setLength(mask_length);
    _validate();
}

IPNetwork::IPNetwork(IPAddress &ipaddr, sfwdr::size_t mask_length){
    _init();
    this->netaddr->set(ipaddr.touInt());
    this->netmask->setLength(mask_length);
    _validate();
}

IPNetwork::~IPNetwork(){
    delete netaddr;
    delete netmask;
    delete[] prefix;
}

void IPNetwork::_init(){
    netaddr = new IPAddress();
    netmask = new IPNetMask();
    prefix = new char[IP_PREFIX_STR_LEN];
    comlib::memset((uint8_t *)prefix, 0, IP_PREFIX_STR_LEN);
}

bool IPNetwork::_validate(){
    if((valid = netmask->isValid())){
        if((netaddr->touInt() & netmask->touInt()) == netaddr->touInt()){
            valid = true;
            
            char plen_buf[3] = {0};
            comlib::uitoa(netmask->getLength(), plen_buf, sizeof(plen_buf));
            comlib::strcat(prefix, netaddr->toStr());
            comlib::strcat(prefix, (char *)"/");
            comlib::strcat(prefix, plen_buf);
            
            return(true);
        }
    }
    valid = false;
    return(false);
}

IPAddress *IPNetwork::getNetAddr(){
    return(netaddr);
}

IPAddress *IPNetwork::getNetMask(){
    return(netmask);
}

bool IPNetwork::isValid(){
    return(valid);
}

char *IPNetwork::toStr(){
    return(prefix);
}
