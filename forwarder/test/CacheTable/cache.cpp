#include <iostream>
#include <string>
#include "CacheTable.hpp"
#include "MacAddress.hpp"
#include <unistd.h>
#include <cstdint>

using namespace std;

class KeyMock{
    int key;
    
public:
    KeyMock(){
        key = 0;
    }
    KeyMock(int v){
        key = v;
    }
    
    int getHash(){
        return key;
    }
    int getKey(){
        return key;
    }
    bool operator==(KeyMock v){
        return(key == v.getKey());
    }
};


int main(void){
    /*
    CacheTable<MacAddress, int *> *tbl = new CacheTable<MacAddress, int *>();
    
    int *n1 = new int(1);
    
    tbl->update(MacAddress("11:11:11:11:11:11"), n1);
    
    printf("%d\n", tbl->getSize());
    sleep(11);
    tbl->refresh();
    printf("%d\n", tbl->getSize());
    
    delete tbl;
    delete n1;
    */
    
    CacheTable<KeyMock, PointerWrapper<int>> tbl;
    
    int *i1 = (int *)malloc(sizeof(int));
    int *i2 = (int *)malloc(sizeof(int));
    int *i3 = (int *)malloc(sizeof(int));
    
    tbl.update(KeyMock(1), PointerWrapper<int>(i1));
    tbl.update(KeyMock(2), PointerWrapper<int>(i2));
    tbl.update(KeyMock(3), PointerWrapper<int>(i3));
    
    printf("%p", tbl.get(KeyMock(4)).unwrap());
    
    sleep(11);
    tbl.refresh();
    
    free(i1);
    free(i2);
    free(i3);
    
}
