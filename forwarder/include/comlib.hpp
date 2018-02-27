#ifndef INCLUDED_COMLIB
#define INCLUDED_COMLIB

#include <cstdint>
/* 少しでもinclude減らしたいけど環境依存しまくるのでやらない
typedef unsigned long int uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef int int32_t;
*/

/**
 * Common Library
 */

namespace sfwdr{
    typedef int32_t ssize_t;
    typedef uint32_t size_t;
}

namespace comlib{
    char *strncpy(char *s1, char *s2, int n);
    char *strncpy(uint8_t *s1, uint8_t *s2, int n);
    
    char *strcat(char *s1, char *s2);
    char *strncat(char *s1, char *s2, int n);
    
    char *strchr(char *s, char c);
    sfwdr::ssize_t strchr_index(char *s, char c);
    
    uint64_t bytestol(uint8_t *head, int n);
    uint8_t *memcpy(uint8_t *buf1, const uint8_t *buf2, int n);
    uint8_t *memmove(uint8_t *buf1, const uint8_t *bud2, uint64_t n);
    uint64_t memset(uint8_t *buf, uint8_t byte, uint64_t n);
    int strlen(char *str);
    
    uint32_t rbyte32(uint32_t in);
    uint16_t rbyte16(uint16_t in);
    
    uint32_t ntohl(uint32_t in);
    uint16_t ntohs(uint16_t in);
    uint32_t htonl(uint32_t in);
    uint16_t htons(uint16_t in);
    
    int atoi(char *s);
    char *uitoa(int n, char *buf, sfwdr::ssize_t buflen);
    
    uint8_t ndigit(uint32_t n);
    inline bool isdigit(char c){ return(c >= '0' && c <= '9'); };
    
    //累乗計算
    uint64_t upow(uint64_t x, uint64_t n);
    
    //指定ビットを取得
    //下位からn bit目、0始まり
    int getBit(uint64_t x, uint8_t n);
}

#endif
