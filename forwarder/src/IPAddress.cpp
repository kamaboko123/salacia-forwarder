#include "IPAddress.hpp"


void IPAddress::_init(){
    addr = 0;
    addr_str = new char[IP_ADDR_STR_LEN]();
    set_flg = false;
}

void IPAddress::_free(){
    delete[] this->addr_str;
}

IPAddress::IPAddress(){
    _init();
    set((uint32_t)0, false);
}

IPAddress::IPAddress(uint32_t addr_uint){
    _init();
    set(addr_uint);
}

IPAddress::IPAddress(char *addr_str){
    _init();
    try{
        set(addr_str);
    }
    catch(sfwdr::Exception::InvalidIPAddress &e){
        _free();
        throw;
    }
}

IPAddress::~IPAddress(){
    _free();
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

void IPAddress::set(uint32_t addr_uint, bool set_flg){
    addr = addr_uint;
    uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);
    this->set_flg = set_flg;
}

void IPAddress::set(char *addr_str, bool set_flg){
    try{
        addr = iptoui(addr_str);
        uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);
    }
    catch(sfwdr::Exception::InvalidIPAddress &e){
        addr = 0;
        uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);
        
        throw e;
    }
    
    this->set_flg = set_flg;
}

void IPAddress::set(const IPAddress &ipaddr){
    set(ipaddr.touInt(), ipaddr.isSet());
}

uint32_t IPAddress::touInt() const{
    return(addr);
}

char *IPAddress::toStr() const{
    return(addr_str);
}

bool IPAddress::isSet() const{
    return(set_flg);
}

char *IPAddress::uitoip(uint32_t addr, char *retbuf, sfwdr::ssize_t retbuf_len){
    if(retbuf_len < IP_ADDR_STR_LEN) return(nullptr);
    
    uint32_t ext_base = 0xFF000000;
    uint8_t ls = 24;
    ssize_t i = 0;
    char buf;
    
    //上位から1byteずつ抜き出して処理
    while(ext_base != 0){
        //flg : hundred's place zero
        bool hp_zero = true;
        
        //100の位(0の場合は入れない)
        buf = (((addr & ext_base) >> ls) / 100) + '0';
        if(buf != '0'){
            hp_zero = false;
            retbuf[i++] = buf;
        }
        
        //10の位
        buf = (((addr & ext_base) >> ls) % 100 / 10) + '0';
        //10の位が0ではない
        // or
        //100の位が0じゃない
        if(buf != '0' || !hp_zero) retbuf[i++] = buf;
        
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
    uint8_t octet = 0;
    uint32_t addr = 0;
    uint64_t tmp;
    char *addr_str_org = addr_str;
    
    while(*addr_str != '\0'){
        tmp = comlib::atoi(addr_str);
        if(comlib::ndigit(tmp) > 3){
            throw sfwdr::Exception::InvalidIPAddress(addr_str_org);
        }
        if(tmp > 255 || tmp < 0){
            throw sfwdr::Exception::InvalidIPAddress(addr_str_org);
        }
        
        //1byte桁上げ + 桁の値加算
        addr = (addr << 8) + tmp;
        
        octet++;
        
        //次のオクテットまでポインタを進める
        do{
            addr_str++;
            if(*addr_str == '\0') break;
            if(*addr_str != '.' && (*addr_str < '0' || *addr_str > '9')){
                //数値とドット以外はエラー
                throw sfwdr::Exception::InvalidIPAddress(addr_str_org);
            }
        }while(*(addr_str - 1) != '.');
    }
    if(octet != 4){
        throw sfwdr::Exception::InvalidIPAddress(addr_str_org);
    }
    
    return(addr);
}

IPNetmask::IPNetmask() : IPAddress(){
    length = 0;
}

IPNetmask::IPNetmask(char *addr_str) : IPAddress(){
    set(addr_str);
}

IPNetmask::IPNetmask(uint32_t addr_uint) : IPAddress(){
    set(addr_uint);
}

sfwdr::ssize_t IPNetmask::set(uint32_t addr_uint){
    try{
        length = validIPNetmask(addr_uint);
        IPAddress::set(addr_uint);
    }
    catch(sfwdr::Exception::Exception &e){
        char buf_ex[32];
        comlib::uitoa(addr_uint, buf_ex, 32);
        throw sfwdr::Exception::InvalidIPNetmask(buf_ex);
    }
    return(getLength());
}

sfwdr::ssize_t IPNetmask::set(char *addr_str){
    try{
        uint32_t addr_uint = iptoui(addr_str);
        set(addr_uint);
    }
    catch(sfwdr::Exception::Exception &e){
        throw sfwdr::Exception::InvalidIPNetmask(addr_str);
    }
    return(getLength());
}

sfwdr::ssize_t IPNetmask::setLength(sfwdr::ssize_t mask_length){
    if(mask_length == 0) return(set((uint32_t)0));
    if(mask_length < 0 || mask_length > 32){
        char ex_buf[64] = "length ";
        char ex[32];
        
        comlib::strncat(ex_buf, comlib::uitoa(mask_length, ex, 32), 64);
        throw sfwdr::Exception::InvalidIPNetmask(ex_buf);
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

sfwdr::size_t IPNetmask::validIPNetmask(uint32_t mask){
    uint8_t length = 0;
    uint32_t cmask = 0x80000000;
    uint8_t buf;
    bool find_0 = false;
    
    for(int i = 32; i > 0; i--){
        buf = (mask & cmask) >> (i-1);
        
        if(find_0 && buf == 1){
            char buf_ex[32];
            comlib::uitoa(mask, buf_ex, 32);
            
            throw sfwdr::Exception::InvalidIPNetmask(buf_ex);
        }
        if(buf == 0) find_0 = true;
        if(!find_0) length++;
        
        cmask = cmask >> 1;
    }
    
    return(length);
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

/*
IPNetwork::IPNetwork(char *addr_str, sfwdr::ssize_t mask_length){
    _init();
    IPAddress ipaddr(addr_str);
    this->netaddr->set(ipaddr.touInt());
    this->netmask->setLength(mask_length);
    _validate();
}*/

IPNetwork::IPNetwork(const IPAddress &ipaddr, sfwdr::ssize_t mask_length){
    _init();
    this->netaddr->set(ipaddr.touInt());
    this->netmask->setLength(mask_length);
    _validate();
}
/*
IPNetwork::IPNetwork(const IPAddress &ipaddr, const IPNetmask &netmask){
    _init();
    this->netaddr->set(ipaddr.touInt());
    this->netmask->set(netmask.touInt());
    _validate();
}*/

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
    //valid = netmask->isValid();
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

/*
bool IPNetwork::set(const IPAddress &ipaddr, const IPNetmask &netmask){
    this->netaddr->set(ipaddr.touInt());
    this->netmask->set(netmask.touInt());
    _validate();
    return(isValid());
}*/

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
    if(*(str = str + prefix_index + 1) == '\0') return(false);
    //数字じゃなけくても不正
    if(!comlib::isdigit(*str)) return(false);
    
    //数値変換して範囲チェック
    //数値の桁数、残りの文字数が一致しているか（無駄な文字が入ってないか）
    int plen = comlib::atoi(str);
    if(plen < 0 && plen > 32) return(false);
    if(comlib::ndigit(plen) != comlib::strlen(str)) return(false);
    
    return(true);
}

bool IPNetwork::validIPNetwork(char *nw_str){
    char *nwaddr_str = nullptr;
    IPAddress *nwaddr = new IPAddress();
    IPNetmask *nwmask = new IPNetmask();
    
    try{
        //ネットワークアドレス
        sfwdr::ssize_t nwaddr_end_index;
        if((nwaddr_end_index = comlib::strchr_index(nw_str, '/')) == -1){
            //マスク無しはエラー
            throw sfwdr::Exception::InvalidIPNetwork((char *)"");
        }
        nwaddr_str = new char[nwaddr_end_index + 1]();
        comlib::strncat(nwaddr_str, nw_str, nwaddr_end_index);
        nwaddr->set(nwaddr_str);
        
        //サブネットマスク
        //残りの文字を数値変換->桁数カウント、残りの文字数と一致するか
        
        int prefix_len_str_len = comlib::strlen(&nw_str[nwaddr_end_index + 1]);
        int plen = comlib::atoi(&nw_str[nwaddr_end_index + 1]);
        if(prefix_len_str_len != comlib::ndigit(plen)){
            throw sfwdr::Exception::InvalidIPNetwork((char *)"");
        }
        
        nwmask->setLength(plen);
        
        //ネットワークアドレスとサブネットマスクの論理積が、ネットワークアドレスと同じであれば正当
        if((nwaddr->touInt() & nwmask->touInt()) != nwaddr->touInt()){
            throw sfwdr::Exception::InvalidIPNetwork((char *)"");
        }
        
        delete[] nwaddr_str;
        delete nwaddr;
        delete nwmask;
    }
    catch(sfwdr::Exception::Exception){
        delete nwaddr;
        delete nwmask;
        if(nwaddr_str != nullptr) delete[] nwaddr_str;
        throw sfwdr::Exception::InvalidIPNetwork(nw_str);
    }
    
    return(true);
}
