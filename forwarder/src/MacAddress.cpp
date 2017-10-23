#include "MacAddress.hpp"

MacAddress::MacAddress(){
    this->addr = 0;
}

/**
 * シスコ表記対応 -> 1122.3344.ffee
 * コロン区切り対応 -> 11:22:33:44:ff:ee
 * 区切り文字なし対応 -> 11223344ffee
 * バリデーション未実装
 */
MacAddress::MacAddress(char *addr_str){
    this->addr = 0;
    char *p = addr_str;
    int hex = 0;
    
    for (p = addr_str; *(p+1) != '\0'; p++){
        if((*p == ':') || (*p == '-') || (*p == '.')){
            continue;
        }
        hex++;
    }
    
    for(p = addr_str; *p != '\0'; p++){
        if((*p >= 'a') && (*p <= 'f')){
            this->addr += ((uint64_t)(*p - 'a' + 10) << (hex * 4));
        }
        else if((*p >= 'A') && (*p <= 'F')){
            this->addr += ((uint64_t)(*p - 'A' + 10) << (hex * 4));
        }
        else if((*p >= '0') && (*p <= '9')){
            this->addr += ((uint64_t)(*p - '0') << (hex * 4));
        }
        else{
            continue;
        }
        hex--;
    }
}

MacAddress::MacAddress(const uint64_t addr_int){
    this->addr = addr_int;
}


[[deprecated("please use toLong() function")]]
uint64_t MacAddress::toInt(){
    return(toLong());
}

uint64_t MacAddress::toLong(){
    return(this->addr);
}

uint64_t MacAddress::getHash(){
    return(this->addr % this->hbase);
}

bool MacAddress::operator==(MacAddress v){
    return(this->toLong() == v.toLong());
}

bool MacAddress::operator!=(MacAddress v){
    return(!(*this == v));
}


//代入演算
//こんな感じに使えるようにする
//MacAddress mac
//mac = 0x112233445566
MacAddress &MacAddress::operator=(const uint64_t addr){
    this->addr = addr;
    return(*this);
}

