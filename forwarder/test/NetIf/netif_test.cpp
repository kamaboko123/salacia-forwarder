#include <iostream>
#include "../../src/NetIf.hpp"

using namespace std;

int main(){
    NetIf *nif = new NetIf((char *)"enp4s0", IFTYPE_L2_ACCESS, 1);
    cout << nif->getMac().toInt() << endl;
    return(0);
}
