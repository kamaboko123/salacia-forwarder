#include <iostream>
#include <string>
#include "CacheTable.hpp"
#include "MacAddress.hpp"
#include "NetIf.hpp"

using namespace std;

int main(void){
    CacheTable<MacAddress, int *> *tbl = new CacheTable<MacAddress, int *>();
    
    int *n1 = new int(1);
    
    tbl->update(MacAddress("11:11:11:11:11:11"), n1);
    
    printf("%d\n", tbl->getSize());
    sleep(11);
    tbl->refresh();
    printf("%d\n", tbl->getSize());
    
    delete tbl;
    delete n1;
}
