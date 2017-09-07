#ifndef CALCADDR_H
#define CALCADDR_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

unsigned int iptoi(char *addr);
uint64_t mactol(uint8_t *addr);
uint8_t *ltomac(uint8_t *ret, uint64_t addr_l);

#endif
