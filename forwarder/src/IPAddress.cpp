#include "IPAddress.hpp"


void IPAddress::_init(){
    addr = 0;
    addr_str = new char[IP_ADDR_STR_LEN];
}

IPAddress::IPAddress(){
    _init();
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
    printf("%u\n", addr);
    
    return(addr);
}

