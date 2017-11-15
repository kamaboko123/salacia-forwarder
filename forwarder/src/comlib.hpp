#ifndef INCLUDED_COMLIB
#define INCLUDED_COMLIB

#include <cstdint>

/**
 * Common Library
 */

namespace sfwdr{
    typedef uint32_t size_t;
}

namespace comlib{
    void strncpy(char *s1, char *s2, int n);
    void strncpy(uint8_t *s1, uint8_t *s2, int n);
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
    
    inline bool isdigit(char c){ return(c >= '0' && c <= '9'); };
}

#endif
