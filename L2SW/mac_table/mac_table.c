#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "../calcaddr.h"
#include "../mynet.h"


#define MAC_TABLE_SIZE 256

struct MAC_TABLE_ENTRY {
    uint64_t mac_addr;
    //struct NETIF netif;
    uint8_t netif_index;
    struct MAC_TABLE_ENTRY *next;
};

struct MAC_TABLE {
    struct MAC_TABLE_ENTRY entry[MAC_TABLE_SIZE];
};

uint8_t hash8(uint64_t value){
    return(value % 256);
}

/* MAC_TABLE 制御関連関数 */
uint8_t update_mac_table(struct MAC_TABLE *tbl, uint8_t *addr, uint8_t netif_index){
    //struct MAC_TABLE_ENTRY *first;
    //first = tbl->entry[hash8(mactol(addr))];
    
    //if(first.mac_addr == mactol(addr)){
    //
    //}
    tbl->entry[hash8(mactol(addr))].mac_addr = mactol(addr);
    tbl->entry[hash8(mactol(addr))].netif_index = netif_index;
}

//struct *MAC_TABLE_ENTRY search_mac_table(void){}

int main(void){
    struct MAC_TABLE mac_table = {0};
    uint64_t val;
    
    update_mac_table(&mac_table, "11:22:33:aa:bb:cc", 1);
    
    //mac_table.entry[hash8(mactol("11:22:33:aa:bb:cd"))].mac_addr = mactol("11:22:33:aa:bb:cd");
    val =  mac_table.entry[hash8(mactol("11:22:33:aa:bb:cd"))].mac_addr;
    printf("%" PRId64 "\n", val);
    val =  mac_table.entry[hash8(mactol("11:22:33:aa:bb:cc"))].mac_addr;
    printf("%" PRId64 "\n", val);
    
    return(0);
}
