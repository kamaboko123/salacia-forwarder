#include "IPAddress.hpp"


void IPAddress::_init(){
    addr = 0;
    addr_str = new char[IP_ADDR_STR_LEN]();
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

IPAddress::IPAddress(const IPAddress &ipaddr){
    _init();
    set(ipaddr);
}

IPAddress &IPAddress::operator=(const IPAddress &ipaddr){
    if(this != &ipaddr){
        set(ipaddr);
    }
    return(*this);
}

void IPAddress::set(uint32_t addr_uint){
    addr = addr_uint;
    uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);
}

void IPAddress::set(char *addr_str){
    addr = iptoui(addr_str);
    uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);
}

void IPAddress::set(const IPAddress &ipaddr){
    set(ipaddr.touInt());
}

uint32_t IPAddress::touInt() const{
    return(addr);
}

char *IPAddress::toStr() const{
    return(addr_str);
}

char *IPAddress::uitoip(uint32_t addr, char *retbuf, sfwdr::ssize_t retbuf_len){
    if(retbuf_len < IP_ADDR_STR_LEN) return(nullptr);
    
    uint32_t ext_base = 0xFF000000;
    uint8_t ls = 24;
    ssize_t i = 0;
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

IPNetmask::IPNetmask() : IPAddress(){
    _validate();
}

IPNetmask::IPNetmask(char *addr_str) : IPAddress(addr_str){
    _validate();
}

IPNetmask::IPNetmask(uint32_t addr_uint) : IPAddress(addr_uint){
    _validate();
}

sfwdr::ssize_t IPNetmask::set(uint32_t addr_uint){
    IPAddress::set(addr_uint);
    _validate();
    return(getLength());
}

sfwdr::ssize_t IPNetmask::set(char *addr_str){
    IPAddress::set(addr_str);
    _validate();
    return(getLength());
}

sfwdr::ssize_t IPNetmask::setLength(sfwdr::ssize_t mask_length){
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

bool IPNetmask::_validate(){
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

bool IPNetmask::isValid() const{
    return(valid);
}

sfwdr::ssize_t IPNetmask::getLength() const{
    return(length);
}

IPNetwork::IPNetwork(){
    _init();
    _validate();
}

IPNetwork::IPNetwork(char *ipnet_str){
    _init();
    set(ipnet_str);
}

IPNetwork::IPNetwork(char *addr_str, sfwdr::ssize_t mask_length){
    _init();
    IPAddress ipaddr(addr_str);
    this->netaddr->set(ipaddr.touInt());
    this->netmask->setLength(mask_length);
    _validate();
}

IPNetwork::IPNetwork(const IPAddress &ipaddr, sfwdr::ssize_t mask_length){
    _init();
    this->netaddr->set(ipaddr.touInt());
    this->netmask->setLength(mask_length);
    _validate();
}

IPNetwork::IPNetwork(const IPAddress &ipaddr, const IPNetmask &netmask){
    _init();
    this->netaddr->set(ipaddr.touInt());
    this->netmask->set(netmask.touInt());
    _validate();
}

IPNetwork::~IPNetwork(){
    delete netaddr;
    delete netmask;
    delete[] prefix;
}


IPNetwork::IPNetwork(const IPNetwork &ipnet){
    _init();
    set(ipnet);
}

IPNetwork &IPNetwork::operator=(const IPNetwork &ipnet){
    if(this != &ipnet){
        set(ipnet);
    }
    return(*this);
}


void IPNetwork::_init(){
    netaddr = new IPAddress((uint32_t)IP_NETWORK_INVALID_NWADDR);
    netmask = new IPNetmask((uint32_t)IP_NETWORK_INVALID_MASK);
    prefix = new char[IP_PREFIX_STR_LEN]();
}

bool IPNetwork::set(char *ipnet_str){
    if(!validPrefixFormat(ipnet_str)){
        //invalidな値を入れる
        this->netaddr->set(IP_NETWORK_INVALID_NWADDR);
        this->netmask->setLength(IP_NETWORK_INVALID_MASK);
    }
    else{
        int pindex = comlib::strchr_index(ipnet_str, '/');
        char *netaddr_str = new char[pindex + 1]();
        comlib::strncpy(netaddr_str, ipnet_str, pindex);
        int plen = comlib::atoi(ipnet_str + pindex + 1);
        
        this->netaddr->set(netaddr_str);
        this->netmask->setLength(plen);
        
        delete[] netaddr_str;
    }
    _validate();
    return(isValid());
}

bool IPNetwork::_validate(){
    valid = netmask->isValid();
    valid = !(netaddr->touInt() == 0 && netmask->touInt() != 0);
    
    comlib::memset((uint8_t *)prefix, 0, sizeof(char)*IP_PREFIX_STR_LEN);
    if(valid){
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

bool IPNetwork::set(const IPNetwork &ipnet){
    this->netaddr->set(ipnet.getNetaddr().touInt());
    this->netmask->set(ipnet.getNetmask().touInt());
    return(_validate());
}

bool IPNetwork::set(const IPAddress &ipaddr, const IPNetmask &netmask){
    this->netaddr->set(ipaddr.touInt());
    this->netmask->set(netmask.touInt());
    _validate();
    return(isValid());
}

IPAddress &IPNetwork::getNetaddr() const{
    return(*netaddr);
}

IPNetmask &IPNetwork::getNetmask() const{
    return(*netmask);
}

bool IPNetwork::isValid() const{
    return(valid);
}

char *IPNetwork::toStr() const{
    return(prefix);
}

//与えられた文字列がIPのPrefixフォーマットか検査
bool IPNetwork::validPrefixFormat(char *str){
    // strから/を探す、見つからなかったら不正
    int prefix_index = comlib::strchr_index(str, '/');
    if(prefix_index == -1) return(false);
    
    //次の文字が終端なら不正
    if((str = str + prefix_index + 1) == '\0') return(false);
    //数字じゃなけくても不正
    if(!comlib::isdigit(*str)) return(false);
    
    //数値変換して範囲チェック
    //数値の桁数、残りの文字数が一致しているか（無駄な文字が入ってないか）
    int plen = comlib::atoi(str);
    if(plen < 0 && plen > 32) return(false);
    if(comlib::ndigit(plen) != comlib::strlen(str)) return(false);
    
    return(true);
}
