#ifndef CALCADDR_H
#define CALCADDR_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

uint32_t iptoi(const char *addr);
uint64_t mactol(uint8_t *addr);
char *ltomac(char *ret, uint64_t addr_l);
uint64_t mactol_bin(uint8_t *addr);

#endif
