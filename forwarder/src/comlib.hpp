#ifndef INCLUDED_COMLIB
#define INCLUDED_COMLIB

#include <cstdint>

/**
 * Common Library
 */

namespace comlib{
    void strncpy(char *s1, char *s2, int n);
    void strncpy(uint8_t *s1, uint8_t *s2, int n);
    uint64_t bytestol(uint8_t *head, int n);
    uint8_t *memcpy(uint8_t *buf1, const uint8_t *buf2, int n);
    int strlen(char *str);
    uint64_t memset(uint8_t *buf, uint8_t byte, uint64_t n);
}

#endif
