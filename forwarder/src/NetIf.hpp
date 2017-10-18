#ifndef INCLUDED_NET_IF
#define INCLUDED_NET_IF

#include <iostream>
#include <cstdint>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include "MacAddress.hpp"
#include "comlib.hpp"

using namespace std;

enum IfType {
    IFTYPE_L2_ACCESS = 1,
    IFTYPE_L2_TRUNK = 2,
    IFTYPE_L3_V4 = 3,
};

class NetIf{
private:
    char ifname[IFNAMSIZ];
    MacAddress *mac_addr;
    IfType iftype;
    uint16_t vlan;
    
    int ifindex;
    struct ifreq ifr;
    struct sockaddr myaddr;
    struct sockaddr_ll sll;
    
public:
    int pd;
    
    NetIf(char *ifname, IfType iftype, uint16_t vlan);
    void initL2aIf(uint16_t vlan);
    MacAddress getMac();
};

#endif
