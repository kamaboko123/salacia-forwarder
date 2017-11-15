#include "comlib.hpp"

/**
 * copy s2 to s1
 */
void comlib::strncpy(char *s1, char *s2, int n){
    bool end = false;
    int i;
    for(i = 0; i < n; i++){
        if(end){
            s1[i] = '\0';
            continue;
        }
        if(s2[i] == '\0'){
            s1[i] = '\0';
            end = true;
        }
        s1[i] = s2[i];
    }
}

void comlib::strncpy(uint8_t *s1, uint8_t *s2, int n){
    comlib::strncpy((char *)s1, (char *)s2, n);
}

/**
 * copy buf2 to buf1
 */
uint8_t *comlib::memcpy(uint8_t *buf1, const uint8_t *buf2, int n){
    for(int i = 0; i < n; i++){
        buf1[i] = buf2[i];
    }
    return buf1;
}

/**
 * move bytes buf2 to buf1
 */
uint8_t *comlib::memmove(uint8_t *buf1, const uint8_t *buf2, uint64_t n){
    uint8_t *tmp = new uint8_t[sizeof(uint8_t) * n];
    memcpy(tmp, buf2, n);
    memcpy(buf1, tmp, n);
    delete[] tmp;
    return(buf1);
}

/**
 * convert byte array to long(uint64_t)
 */
uint64_t comlib::bytestol(uint8_t *head, int n){
    uint64_t ret = 0;
    for(int i = 0; i < n; i++){
        ret += ((uint64_t)head[i] << (8 * (n - i - 1)));
    }
    return(ret);
}

/**
 * return length of str
 */
int comlib::strlen(char *str){
    int i;
    for(i = 0; str[i] != '\0'; i++);
    return(i + 1);
}

/**
 * set [byte] to memory from [buf] to [n] byte.
 * return set num
 */
uint64_t comlib::memset(uint8_t *buf, uint8_t byte, uint64_t n){
    for(uint64_t i = 0; i < n; i++){
        buf[i] = byte;
    }
    return(n);
}

/**
 * convert byte order
 */

//[32bit] Host byte order to Network byte order
uint32_t comlib::rbyte32(uint32_t in){
    uint32_t ret = 0;
    uint8_t *rp = (uint8_t *)&ret;
    uint8_t *ip = (uint8_t *)&in;
    
    for(int i = 0; i < 4; i++){
        rp[i] = ip[3 - i];
    }
    return(ret);
}

//[16bit] Host byte order to Network byte order
uint16_t comlib::rbyte16(uint16_t in){
    uint32_t ret = 0;
    uint8_t *rp = (uint8_t *)&ret;
    uint8_t *ip = (uint8_t *)&in;
    
    rp[0] = ip[1];
    rp[1] = ip[0];
    
    return(ret);
}

uint32_t comlib::ntohl(uint32_t in){
    return(rbyte32(in));
}
uint16_t comlib::ntohs(uint16_t in){
    return(rbyte16(in));
}
uint32_t comlib::htonl(uint32_t in){
    return(rbyte32(in));
}
uint16_t comlib::htons(uint16_t in){
    return(rbyte16(in));
}

//not implement orverflow checking
int comlib::atoi(char *s){
    int result = 0;
    bool sign = false;
    
    //符号付き
    if(*s == '-'){
        sign = true;
        s++;
    }
    
    while(isdigit(*s)){
        //すでに入ってるものを桁上げ + その桁の数値を加算
        result = (result * 10) + (*s - '0');
        s++;
    }
    
    if (sign){
        result = -result;
    }
    
    return result;
}

