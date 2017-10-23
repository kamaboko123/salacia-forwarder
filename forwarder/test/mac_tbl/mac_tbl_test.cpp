#include <iostream>
#include "../../src/HashMap.hpp"
#include "../../src/MacAddress.hpp"
#include "../../src/NetIf.hpp"

using namespace std;

int main(void){
    cout << "Test : MacTable" << endl;
    
    HashMap<MacAddress, NetIf *> *mac_tbl = new HashMap<MacAddress, NetIf *>(256);
    NetIf *nif1 = new NetIf((char *)"enp3s0", IFTYPE_L2_ACCESS, 1);
    NetIf *nif2 = new NetIf((char *)"enp4s0", IFTYPE_L2_ACCESS, 1);
    
    mac_tbl->update(MacAddress((char *)"11:22:33:44:55:66"), nif1);
    mac_tbl->update(MacAddress((char *)"11:22:33:44:55:55"), nif1);
    mac_tbl->update(MacAddress((char *)"11:22:33:44:55:11"), nif1);
    
    mac_tbl->update(MacAddress((char *)"11:22:33:44:55:61"), nif2);
    mac_tbl->update(MacAddress((char *)"11:22:33:44:55:51"), nif2);
    mac_tbl->update(MacAddress((char *)"11:22:33:44:55:21"), nif2);
    
    cout << mac_tbl->get(MacAddress((char *)"11:22:33:44:55:66"))->getMac().toInt() << endl;
    cout << mac_tbl->get(MacAddress((char *)"11:22:33:44:55:55"))->getMac().toInt() << endl;
    cout << mac_tbl->get(MacAddress((char *)"11:22:33:44:55:11"))->getMac().toInt() << endl;
    
    cout << mac_tbl->get(MacAddress((char *)"11:22:33:44:55:61"))->getMac().toInt() << endl;
    cout << mac_tbl->get(MacAddress((char *)"11:22:33:44:55:51"))->getMac().toInt() << endl;
    cout << mac_tbl->get(MacAddress((char *)"11:22:33:44:55:21"))->getMac().toInt() << endl;
    
    delete mac_tbl;
    delete nif1;
    delete nif2;
    
    return(0);
}
