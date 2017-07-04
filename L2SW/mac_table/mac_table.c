#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "../calcaddr.h"
#include "../mynet.h"

#define HASH_SIZE 256

struct MAC_TABLE_ENTRY {
    uint64_t mac_addr;
    uint8_t netif_index;
    uint64_t last_time;
    struct MAC_TABLE_ENTRY *next;
    struct MAC_TABLE_ENTRY *prev;
};

struct MAC_TABLE {
    struct MAC_TABLE_ENTRY entry[HASH_SIZE];
    //struct MAC_TABLE_ENTRY *entry;
};

uint8_t hash8(uint64_t value){
    return(value % 256);
}

void init_entry(struct MAC_TABLE_ENTRY *entry);
void refresh_mac_table(struct MAC_TABLE *tbl);

/* MAC_TABLE 制御関連関数 */
uint8_t update_mac_table(struct MAC_TABLE *tbl, uint8_t *addr, uint8_t netif_index){
    struct MAC_TABLE_ENTRY *first;
    struct MAC_TABLE_ENTRY *p = NULL;
    first = &tbl->entry[hash8(mactol(addr))];
    
    if(first->mac_addr == 0) {
        //printf("not found!\n");
        tbl->entry[hash8(mactol(addr))].mac_addr = mactol(addr);
        tbl->entry[hash8(mactol(addr))].netif_index = netif_index;
        tbl->entry[hash8(mactol(addr))].last_time = time(NULL);
        
        return(0);
    }
    else if(first->mac_addr == mactol(addr)){
        tbl->entry[hash8(mactol(addr))].mac_addr = mactol(addr);
        tbl->entry[hash8(mactol(addr))].netif_index = netif_index;
        tbl->entry[hash8(mactol(addr))].last_time = time(NULL);
        return(0);
    }
    else{
        //printf("?\n");
        p = first;
        while(p->next != NULL){
            p = p->next;
            if(p->mac_addr == mactol(addr)){
                p->mac_addr = mactol(addr);
                p->netif_index = netif_index;
                p->last_time = time(NULL);
                return(0);
            }
        }
        
        p->next = malloc(sizeof(struct MAC_TABLE_ENTRY));
        p->next->mac_addr = mactol(addr);
        p->next->netif_index = netif_index;
        p->next->last_time = time(NULL);
        p->next->next = NULL;
        p->next->prev = p;
        return(0);
    }
}

struct MAC_TABLE_ENTRY *get_mac_entry(struct MAC_TABLE *tbl, uint8_t *addr){
    struct MAC_TABLE_ENTRY *p;
    p = &tbl->entry[hash8(mactol(addr))];
    
    if(p->mac_addr == mactol(addr)){
        p->last_time = time(NULL);
        return(p);
    }
    while(p->next != NULL){
        p = p->next;
        if(p->mac_addr == mactol(addr)){
            p->last_time = time(NULL);
            return(p);
        }
    }
    return(NULL);
}

uint8_t delete_mac_entry(struct MAC_TABLE *tbl, uint8_t *addr){
    struct MAC_TABLE_ENTRY *target;
    target = get_mac_entry(tbl, addr);
    if(target == NULL){
        printf("not found![%s]\n", addr);
    }
    else{
        //first entry
        if(target->prev == NULL){
            if(target->next == NULL){
                //最初のエントリかつ次のエントリがない
                init_entry(target);
                return(0);
            }
            else{
                //最初のエントリ、次のエントリ有り
                tbl->entry[hash8(mactol(addr))].mac_addr = target->next->mac_addr;
                tbl->entry[hash8(mactol(addr))].netif_index = target->next->netif_index;
                tbl->entry[hash8(mactol(addr))].last_time = target->next->last_time;
                tbl->entry[hash8(mactol(addr))].next = NULL;
                //tbl->entry[hash8(mactol(addr))].prev = NULL;
                free(target->next);
            }
        }else{
            if(target->next == NULL){
                //最後のエントリ(最初のエントリではなく、後ろに続くエントリがない)
                target->prev->next = NULL;
                free(target);
            }
            else{
                //中間のエントリ（最初のエントリではなく、後ろに続くエントリが存在する）
                //target->prev->mac_addr = target->next->mac_addr;
                //target->prev->netif_index = target->next->netif_index;
                //target->prev->last_time = target->next->last_time;
                target->prev->next = target->next;
                free(target);
            }
        }
        //printf("found!\n");
    }
    return(0);
}

/*
struct MAC_TABLE_ENTRY[] *find_expire_entry(tbl){

}*/


struct MAC_TABLE_ENTRY *delete_chain(struct MAC_TABLE_ENTRY *entry){
    struct MAC_TABLE_ENTRY *ret;
    if(entry->next == NULL){
        return(entry);
    }
    ret = entry->prev;
    delete_chain(entry);
    return(ret);
}


void refresh_mac_table(struct MAC_TABLE *tbl){
    int i;
    struct MAC_TABLE_ENTRY *p;
    //struct MAC_TABLE_ENTRY *p2;
    uint64_t now = time(NULL);
    uint8_t buf[36] = {0};
    
    for(i=0; i<HASH_SIZE; i++){
        p = &tbl->entry[i];
        do{
            //if(p->mac_addr != 0){
                ltomac(buf, p->mac_addr);
                if((now - p->last_time > 0) && p->mac_addr != 0){
                    printf("delete : %s\n", buf);
                }
                p = p->next;
                delete_mac_entry(tbl, buf);
            //}
            
            /*
            if(now - p->prev->last_time > 0){
                if(p->prev->mac_addr != 0){
                    ltomac(buf, p->prev->mac_addr);
                    delete_mac_entry(tbl, buf);
                    printf("delete : %s\n", buf);
                }
            }*/
        }while(p != NULL);
    }
}

void init_entry(struct MAC_TABLE_ENTRY *entry){
    entry->next = NULL;
    entry->prev = NULL;
    entry->mac_addr = 0;
    entry->netif_index = 0;
    entry->last_time = 0;
}

void init_mac_table(struct MAC_TABLE *tbl){
    int i;
    
    for(i = 0; i < HASH_SIZE; i++){
        init_entry(&tbl->entry[i]);
        //printf("%d\n", i);
        //tbl->entry[i].next = NULL;
        //tbl->entry[i].prev = NULL;
    }
}

void dump_mac_entry(struct MAC_TABLE_ENTRY *entry){
    //printf("{%" PRId64 ", %d} %s ", entry->mac_addr, entry->netif_index, (entry->next == NULL ? "-> NULL" : "->"));
    uint8_t mac_str[18];
    ltomac(mac_str, entry->mac_addr);
    printf("{%s, %d, %" PRIu64 "} %s ", mac_str, entry->netif_index, entry->last_time, (entry->next == NULL ? "-> NULL" : "->"));
}

void dump_mac_table(struct MAC_TABLE *tbl){
    struct MAC_TABLE_ENTRY *p;
    int i;
    
    for(i = 0; i < HASH_SIZE; i++){
        p = NULL;
        if (tbl->entry[i].mac_addr != 0){
            //dump_mac_entry(&tbl->entry[i]);
            printf("[%d] ", i);
            p = &tbl->entry[i];
            while(p != NULL){
                dump_mac_entry(p);
                p = p->next;
            }
            printf("\n");
        }
    }
    printf("\n");
}

//struct *MAC_TABLE_ENTRY search_mac_table(void){}

int main(void){
    struct MAC_TABLE mac_table = {0};
    struct MAC_TABLE_ENTRY *tmp;
    uint64_t val;
    
    init_mac_table(&mac_table);
    
    update_mac_table(&mac_table, "01:22:33:aa:bb:cc", 1);
    update_mac_table(&mac_table, "11:22:33:aa:ba:1c", 2);
    update_mac_table(&mac_table, "11:22:33:aa:b9:dc", 3);
    update_mac_table(&mac_table, "11:22:33:af:b8:cc", 4);
    update_mac_table(&mac_table, "11:22:33:aa:b7:cd", 5);
    update_mac_table(&mac_table, "11:22:33:aa:b6:c3", 6);
    update_mac_table(&mac_table, "11:22:33:aa:b5:c2", 7);
    update_mac_table(&mac_table, "11:22:33:aa:b4:c6", 8);
    update_mac_table(&mac_table, "11:22:33:aa:b3:cf", 9);
    update_mac_table(&mac_table, "11:22:33:aa:b2:ca", 10);
    update_mac_table(&mac_table, "11:22:33:aa:b1:cd", 11);
    update_mac_table(&mac_table, "11:22:33:aa:b0:c2", 12);
    update_mac_table(&mac_table, "11:22:33:aa:c0:cd", 13);
    
    dump_mac_table(&mac_table);
    
    //delete_mac_entry(&mac_table, "11:22:33:aa:b5:c2");
    //delete_mac_entry(&mac_table, "11:22:33:aa:ba:1c");
    //delete_mac_entry(&mac_table, "11:22:33:af:b8:cc");
    //delete_mac_entry(&mac_table, "11:22:33:aa:b1:cd");
    //dump_mac_table(&mac_table);
    
    //sleep(2);
    //delete_chain(&mac_table.entry[205]);
    refresh_mac_table(&mac_table);
    dump_mac_table(&mac_table);
    
    
    printf("---\n");
    //tmp = get_mac_entry(&mac_table, "11:22:33:aa:bb:cc");
    //printf("%" PRId64 ", %d\n", tmp->mac_addr, tmp->netif_index);
    
    //tmp = get_mac_entry(&mac_table, "11:22:33:aa:ba:cc");
    //printf("%" PRId64 ", %d\n", tmp->mac_addr, tmp->netif_index);
    
    //mac_table.entry[hash8(mactol("11:22:33:aa:bb:cd"))].mac_addr = mactol("11:22:33:aa:bb:cd");
    //val =  mac_table.entry[hash8(mactol("11:22:33:aa:bb:cd"))].mac_addr;
    //printf("%" PRId64 "\n", val);
    //val =  mac_table.entry[hash8(mactol("11:22:33:aa:bb:cc"))].mac_addr;
    //printf("%" PRId64 "\n", val);
    //val =  mac_table.entry[hash8(mactol("11:22:33:aa:bc:cc"))].mac_addr;
    //printf("%" PRId64 "\n", val);
    
    return(0);
}
