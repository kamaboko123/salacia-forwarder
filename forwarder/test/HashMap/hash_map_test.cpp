#include <iostream>
#include <string>
#include "HashMap.hpp"
#include "MacAddress.hpp"
#include "NetIf.hpp"

using namespace std;

template<>
int HashMap<int, int *>::hash(int key) const{
    return(key % this->size);
}

void _dump(HashMap<MacAddress, int*> *map){
    MacAddress *keys = map->getKeys();
    
    printf("size:%d\n", map->getSize());
    
    for(int i = 0; i < map->getSize(); i++){
        cout << keys[i].toLong();
        cout << ":";
        cout << *map->get(keys[i]) << endl;
        
        //cout << sizeof(keys)/sizeof(MacAddress) << endl;
    }
    
    delete[] keys;
}

int main(void){
    
    //int *ii =  new int[10];
    //delete[] ii;
    //delete &ii[0];
    //delete &ii[1];
    
    /*
    //HashMap<int, int> *map1 = new HashMap<int, int>(256);
    HashMap<MacAddress, NetIf *> *map2 = new HashMap<MacAddress, NetIf *>(256);
    MacAddress mac((char *)"11:22:33:44:55:66");
    NetIf *nif = new NetIf((char *)"enp4s0", IFTYPE_L2_ACCESS, 1);
    
    map2->add(mac, nif);
    cout << map2->get(mac)->getMac().toInt() << endl;
    map2->_dump();
    delete map2;
    */
    HashMap<MacAddress, int *> *map = new HashMap<MacAddress, int *>(256);
    
    MacAddress m1((char *)"11:22:33:44:55:61");
    int *pi1 = new int(1);
    map->update(m1, pi1);
    
    MacAddress m2((char *)"11:22:33:44:55:62");
    int *pi2 = new int(2);
    map->update(m2, pi2);
    
    MacAddress m3((char *)"11:22:33:44:55:63");
    int *pi3 = new int(3);
    map->update(m3, pi3);
    
    MacAddress m4((char *)"11:22:33:44:55:64");
    int *pi4 = new int(4);
    map->update(m4, pi4);
    
    MacAddress m5((char *)"11:22:33:44:51:63");
    int *pi5 = new int(5);
    map->update(m5, pi5);
    
    MacAddress m6((char *)"11:22:33:44:52:63");
    int *pi6 = new int(6);
    map->update(m6, pi6);
    
    map->_dump();
    _dump(map);
    
    MacAddress m7((char *)"11:22:33:44:52:63");
    int *pi7 = new int(7);
    map->update(m7, pi7);
    
    //cout << *map->get(m5) << endl;
    
    map->_dump();
    _dump(map);
    
    printf("------\n");
    HashMap<int, int *> *map_int = new HashMap<int, int *>(256);
    map_int->update(10, pi1);
    map_int->update(11, pi2);
    
    printf("!!!!!!!!!\n");
    int * keys = map_int->getKeys();
    for(int i = 0; i < map_int->getSize(); i++){
        printf("%d : %d\n", keys[i], *map_int->get(keys[i]));
    }
    map_int->update(10, pi1);
    delete[] keys;
    printf("!!!!!!!!!\n");
    
    map_int->del(10);
    map_int[10] = pi1;
    
    if(map_int->isExist(10)){
        printf("key : [%d] is exist! -> %d\n", 10, map_int[10]);
    }
    else{
        printf("key : [%d] is not exist!\n", 10);
    }
    
    /*
    map->del(m5);
    map->_dump();
    _dump(map);
    */
    //map->del(m3);
    
    //map->_dump();
    //_dump(map);
    
    /*
    cout << map->getSize() << endl;
    cout << *map->get(m1) << endl;
    */
    
    /*
    MacAddress *keys = map->getKeys();
    //MacAddress *keys = new MacAddress[map->getSize()];
    //map->getKeys(keys);
    cout << endl;
    for(int i = 0; i < map->getSize(); i++){
        //cout << keys[i].toInt() << endl;
        cout << keys[i].toInt();
        cout << ":";
        cout << *map->get(keys[i]) << endl;
    }
    
    map->del(m3);
    map->_dump();
    */
    
    delete map;
    delete map_int;
    delete pi1;
    delete pi2;
    delete pi3;
    delete pi4;
    delete pi5;
    delete pi6;
    delete pi7;
    return(0);
}
