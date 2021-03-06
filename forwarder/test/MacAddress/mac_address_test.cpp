#include <iostream>
#include <cstdint>
#include <string>
#include "MacAddress.hpp"

using namespace std;

int main(void){
    //11:22:33:44:ff:ee -> 18838586720238
    
    const uint64_t correct = 18838586720238;
    const uint64_t hash_correct = 238;
    bool ng = false;
    
    MacAddress *mac = new MacAddress((char *)"11:22:33:44:ff:ee");
    if(mac->toInt() != correct){
        ng = true;
    }
    delete mac;
    
    mac = new MacAddress((char *)"11:22:33:44:FF:EE");
    if(mac->toInt() != correct){
        ng = true;
    }
    delete mac;
    
    mac = new MacAddress((char *)"11223344FFee");
    if(mac->toInt() != correct){
        ng = true;
    }
    delete mac;
    
    mac = new MacAddress((char *)"1122-3344-FfEe");
    if(mac->toInt() !=  correct){
        ng = true;
    }
    delete mac;
    
    mac = new MacAddress((char *)"1122.3344.ffEe");
    if(mac->toInt() !=  correct){
        ng = true;
    }
    
    MacAddress mac_static = (uint64_t)18838586720238;
    if(mac_static.toInt() !=  correct){
        ng = true;
    }
    
    mac_static = (uint64_t)18838586720239;
    if(mac_static.toInt() !=  correct + 1){
        ng = true;
    }
    
    
    if (mac->getHash() != hash_correct){
        ng = true;
    }
    delete mac;
    
    if(ng){
        cout << "NG!" << endl;
        return(1);
    }
    
    cout << "OK!" << endl;
    return(0);
}
