#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "../calcaddr.h"
#include "../mynet.h"


#define HASH_SIZE 256

struct MAC_TABLE_ENTRY {
    uint64_t mac_addr;
    //struct NETIF netif;
    uint8_t netif_index;
    struct MAC_TABLE_ENTRY *next;
};

struct MAC_TABLE {
    struct MAC_TABLE_ENTRY entry[HASH_SIZE];
    //struct MAC_TABLE_ENTRY *entry;
};

uint8_t hash8(uint64_t value){
    return(value % 256);
}

/* MAC_TABLE 制御関連関数 */
uint8_t update_mac_table(struct MAC_TABLE *tbl, uint8_t *addr, uint8_t netif_index){
    struct MAC_TABLE_ENTRY *first;
    struct MAC_TABLE_ENTRY *p = NULL;
    first = &tbl->entry[hash8(mactol(addr))];
    
    if(first->mac_addr == 0) {
        //printf("not found!\n");
        tbl->entry[hash8(mactol(addr))].mac_addr = mactol(addr);
        tbl->entry[hash8(mactol(addr))].netif_index = netif_index;
        return(0);
    }
    else if(first->mac_addr == mactol(addr)){
        tbl->entry[hash8(mactol(addr))].mac_addr = mactol(addr);
        tbl->entry[hash8(mactol(addr))].netif_index = netif_index;
        return(0);
    }
    else{
        printf("?\n");
        p = first;
        while(p->next != NULL){
            p = p->next;
            if(p->mac_addr == mactol(addr)){
                p->mac_addr = mactol(addr);
                p->netif_index = netif_index;
                return(0);
            }
        }
        
        p->next = malloc(sizeof(struct MAC_TABLE_ENTRY));
        p->next->mac_addr = mactol(addr);
        p->next->netif_index = netif_index;
        p->next->next = NULL;
        return(0);
    }
}

struct MAC_TABLE_ENTRY *get_mac_entry(struct MAC_TABLE *tbl, uint8_t *addr){
    struct MAC_TABLE_ENTRY *p;
    p = &tbl->entry[hash8(mactol(addr))];
    
    if(p->mac_addr == mactol(addr)){
        return(p);
    }
    while(p->next != NULL){
        p = p->next;
        if(p->mac_addr == mactol(addr)){
            return(p);
        }
    }
    return(NULL);
}

void init_mac_table(struct MAC_TABLE *tbl){
    int i;
    
    for(i = 0; i < HASH_SIZE; i++){
        //printf("%d\n", i);
        tbl->entry[i].next = NULL;
    }
}


//struct *MAC_TABLE_ENTRY search_mac_table(void){}

int main(void){
    struct MAC_TABLE mac_table = {0};
    struct MAC_TABLE_ENTRY *tmp;
    uint64_t val;
    
    init_mac_table(&mac_table);
    
    update_mac_table(&mac_table, "11:22:33:aa:bb:cc", 1);
    update_mac_table(&mac_table, "11:22:33:aa:ba:cc", 2);
    update_mac_table(&mac_table, "11:22:33:aa:b9:cc", 3);
    update_mac_table(&mac_table, "11:22:33:aa:b8:cc", 4);
    update_mac_table(&mac_table, "11:22:33:aa:b7:cc", 5);
    update_mac_table(&mac_table, "11:22:33:aa:b6:cc", 6);
    update_mac_table(&mac_table, "11:22:33:aa:b5:cc", 7);
    update_mac_table(&mac_table, "11:22:33:aa:b4:cc", 8);
    update_mac_table(&mac_table, "11:22:33:aa:b3:cc", 9);
    update_mac_table(&mac_table, "11:22:33:aa:b2:cc", 10);
    update_mac_table(&mac_table, "11:22:33:aa:b1:cc", 11);
    update_mac_table(&mac_table, "11:22:33:aa:b0:cc", 12);
    
    tmp = get_mac_entry(&mac_table, "11:22:33:aa:bb:cc");
    printf("%" PRId64 ", %d\n", tmp->mac_addr, tmp->netif_index);
    
    tmp = get_mac_entry(&mac_table, "11:22:33:aa:ba:cc");
    printf("%" PRId64 ", %d\n", tmp->mac_addr, tmp->netif_index);
    
    //mac_table.entry[hash8(mactol("11:22:33:aa:bb:cd"))].mac_addr = mactol("11:22:33:aa:bb:cd");
    //val =  mac_table.entry[hash8(mactol("11:22:33:aa:bb:cd"))].mac_addr;
    //printf("%" PRId64 "\n", val);
    //val =  mac_table.entry[hash8(mactol("11:22:33:aa:bb:cc"))].mac_addr;
    //printf("%" PRId64 "\n", val);
    //val =  mac_table.entry[hash8(mactol("11:22:33:aa:bc:cc"))].mac_addr;
    //printf("%" PRId64 "\n", val);
    
    return(0);
}
