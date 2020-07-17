#ifndef INCLUDED_NET_IF
#define INCLUDED_NET_IF

#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cstdint>
#include <iostream>

#include "Ethernet.hpp"
#include "IPAddress.hpp"
#include "MacAddress.hpp"
#include "comlib.hpp"
#include "dlib.hpp"

using namespace std;

enum IfType {
    IFTYPE_L2_ACCESS = 1,
    IFTYPE_L2_TRUNK = 2,
    IFTYPE_L3_V4 = 3,
};

class NetIf {
   private:
    char ifname[IFNAMSIZ];
    MacAddress mac_addr;
    IfType iftype;

    //Layer2
    uint16_t vlan;

    //Layer3
    IPAddress ip_addr;
    IPNetmask ip_mask;

    int pd;
    int ifindex;
    struct ifreq ifr;
    struct sockaddr myaddr;
    struct sockaddr_ll sll;

    void initIf();
    //int sendL2a(uint8_t *data, int length);
    int _send(Ethernet &packet, uint16_t vlan = 0);

   public:
    NetIf();

    //Layer2
    NetIf(char *ifname, IfType iftype, uint16_t vlan);

    //Layer3
    NetIf(char *ifname, IPAddress addr, IPNetmask mask);

    ~NetIf();

    int getFD();

    MacAddress getMac();
    IfType getIfType();
    char *getIfName();
    uint16_t getVlanId();

    IPAddress getIP();
    IPNetmask getIPNetmask();

    int send(Ethernet packet, uint16_t vlan = 0);
    //int send(uint8_t *data, int length, uint16_t vlan = 1);
    int sendRaw(uint8_t *data, int length);
    int sendBroadcast(Ethernet packet, uint16_t vlan = 0);

    uint16_t recvRaw(uint8_t *buf, uint16_t buflen);
    uint16_t recv(Ethernet *eth);
};

#endif
