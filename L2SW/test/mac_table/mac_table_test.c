#include "../../src/mac_table.h"

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
    //delete_mac_entry(&mac_table, "11:22:33:aa:c0:cd");
    //delete_mac_entry(&mac_table, "11:22:33:aa:b7:cd");

    //sleep(2);
    //delete_chain(&mac_table.entry[205]);

    //refresh_mac_table(&mac_table);
    //dump_mac_table(&mac_table);

    sleep(6);
    refresh_mac_table(&mac_table);
    update_mac_table(&mac_table, "01:22:33:aa:bb:cc", 100);
    refresh_mac_table(&mac_table);

    printf("---\n");
    dump_mac_table(&mac_table);
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
