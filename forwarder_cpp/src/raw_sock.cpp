#include <iostream>
#include <string>
#include <unordered_map>
#include "NetIf.hpp"

#include <poll.h>

using namespace std;

int main(){
    struct pollfd pfds[2];
    
    std::cout << "Hello, World!" << std::endl;
    
    unordered_map<string, NetIf *> mp;
    
    //IF初期化、pollで使う
    mp["if1"] = new NetIf("enp4s0", IFTYPE_L2_ACCESS, 1);
    cout << mp["if1"]->getMacStr() << endl;
    
    pfds[0].fd = mp["if1"]->pd;
    pfds[0].events = POLLIN|POLLERR;
    
    return (0);
}
