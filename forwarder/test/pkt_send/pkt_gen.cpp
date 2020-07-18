#include <poll.h>

#include <cinttypes>

#include "Ethernet.hpp"
#include "NetIf.hpp"
#include "RouteTable.hpp"
#include "comlib.hpp"
#include "dlib.hpp"

void dump_netif_info(NetIf &netif) {
    printf("Interface Name: %s\n", netif.getIfName());
    printf("Interface IPAddr: %s\n", netif.getIP().toStr());
    printf("Interface IPNetmask(length): %s(%d)\n", netif.getIPNetmask().toStr(), netif.getIPNetmask().getLength());
    printf("Interface MacAddr: %lx\n", netif.getMac().toLong());
    printf("\n");
}

void update_udp_checksum(Ethernet &eth) {
}

#define IP_SIZE 1291
int main(void) {
    NetIf *netif = new NetIf((char *)"h-ns11", IPAddress((char *)"172.18.0.1"), IPNetmask((char *)"255.255.255.0"));
    dump_netif_info(*netif);

    Ethernet *pkt = new Ethernet();
    pkt->set(PKT_IPV4, sizeof(PKT_IPV4));
    pkt->setSrc(netif->getMac());
    pkt->setDst(MacAddress("36:22:4e:1b:4b:a2"));

    const int ip_payload_size = IP_SIZE - 20;
    //uint8_t payload[1500];

    for (int i = 0; i < ip_payload_size; i++) {
        //payload[i] = i % 255;
        pkt->ipv4().getPayloadPtr()[i] = i % 255;
        //printf("%d: 0x%02x\n", i, payload[i]);
    }
    pkt->ipv4().setProtocol(IP_PROTOCOL::PROTOCOL_UDP);
    //pkt->ipv4().setProtocol(0xFE);
    pkt->ipv4().setLength(IP_SIZE);
    pkt->ipv4().calcHeaderChecksum();
    printf("IP Header checksum: %02x\n", pkt->ipv4().getHeaderChecksum());
    pkt->setLength(IP_SIZE + ETH_H_SIZE);

    dlib::hexdump(pkt->RawData(), IP_SIZE + ETH_H_SIZE);
    netif->send(*pkt);

    delete pkt;
    delete netif;

    return (0);
}
