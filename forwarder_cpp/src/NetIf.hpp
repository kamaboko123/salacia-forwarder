#ifndef INCLUDED_NET_IF
#define INCLUDED_NET_IF

#include <iostream>
#include <string>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

using namespace std;

enum IfType {
    IFTYPE_L2_ACCESS = 1,
    IFTYPE_L2_TRUNK = 2,
    IFTYPE_L3_V4 = 3,
};

class NetIf{
private:
    string ifname;
    IfType iftype;
    uint16_t vlan;
    
    int ifindex;
    struct ifreq ifr;
    struct sockaddr myaddr;
    struct sockaddr_ll sll;
    

public:
    int pd;
    
    NetIf(string ifname, IfType iftype, uint16_t vlan);
    void initL2aIf(uint16_t vlan);
    string getMacStr();
};

#endif
