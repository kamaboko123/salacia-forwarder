#ifndef MACTABLE_H
#define MACTABLE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include "../calcaddr.h"
#include "../mynet.h"

#define HASH_SIZE 256
#define MAC_TABLE_AGING_TIME 5

struct MAC_TABLE_ENTRY {
    uint64_t mac_addr;
    uint8_t netif_index;
    uint64_t last_time;
    struct MAC_TABLE_ENTRY *next;
    struct MAC_TABLE_ENTRY *prev;
};

struct MAC_TABLE {
    struct MAC_TABLE_ENTRY entry[HASH_SIZE];
};

uint8_t hash8(uint64_t value);
uint8_t update_mac_table(struct MAC_TABLE *tbl, uint8_t *addr, uint8_t netif_index);
struct MAC_TABLE_ENTRY *get_mac_entry(struct MAC_TABLE *tbl, uint8_t *addr);
uint8_t delete_mac_entry(struct MAC_TABLE *tbl, uint8_t *addr);
void refresh_mac_table(struct MAC_TABLE *tbl);
void init_entry(struct MAC_TABLE_ENTRY *entry);
void init_mac_table(struct MAC_TABLE *tbl);
void dump_mac_entry(struct MAC_TABLE_ENTRY *entry);
void dump_mac_table(struct MAC_TABLE *tbl);

#endif
