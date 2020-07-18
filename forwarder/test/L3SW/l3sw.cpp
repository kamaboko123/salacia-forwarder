#include <poll.h>

#include <cinttypes>

#include "Ethernet.hpp"
#include "NetIf.hpp"
#include "RouteTable.hpp"
#include "comlib.hpp"
#include "dlib.hpp"

#define NETIF_NUM 2

void reply_arp(NetIf &netif, Ethernet &pkt) {
    uint8_t arp_base[] = PKT_ARP;
    Ethernet arp_reply(arp_base, PKT_ARP_SIZE);

    arp_reply.arp().setSrcMac(netif.getMac());
    arp_reply.arp().setDstMac(pkt.arp().getSrcMac());

    arp_reply.arp().setSrcIP(netif.getIP());
    arp_reply.arp().setDstIP(pkt.arp().getSrcIP());

    arp_reply.arp().setOPCode(ARPOPCode::ARP_REPLY);
    arp_reply.setDst(pkt.arp().getSrcMac());

    printf("[Send ARP reply] interface: %s\n", netif.getIfName());
    printf("%s is at %" PRIx64 "\n", netif.getIP().toStr(), netif.getMac().toLong());
    netif.send(arp_reply);
}

void request_arp(NetIf &netif, Ethernet &pkt) {
    uint8_t arp_base[] = PKT_ARP;
    Ethernet arp_request(arp_base, PKT_ARP_SIZE);

    arp_request.arp().setSrcMac(netif.getMac());
    arp_request.arp().setDstMac(MacAddress((uint64_t)0));
    arp_request.arp().setSrcIP(netif.getIP());
    arp_request.arp().setDstIP(pkt.ipv4().getDst());
    arp_request.arp().setOPCode(ARPOPCode::ARP_REQUEST);
    arp_request.setDst(MacAddress(0xFFFFFFFFFFFF));
    arp_request.setSrc(netif.getMac());

    printf("[Send ARP Request] interface: %s\n\n", netif.getIfName());
    printf("Who has %s ?\n", pkt.ipv4().getDst().toStr());
    netif.send(arp_request);
}

void dump_netif_info(NetIf &netif) {
    printf("Interface Name: %s\n", netif.getIfName());
    printf("Interface IPAddr: %s\n", netif.getIP().toStr());
    printf("Interface IPNetmask(length): %s(%d)\n", netif.getIPNetmask().toStr(), netif.getIPNetmask().getLength());
    printf("Interface MacAddr: %lx\n", netif.getMac().toLong());
    printf("\n");
}

int main(void) {
    CacheTable<IPAddress, MacAddress> arp_tbl(256, 300);
    CacheTable<MacAddress, NetIf *> mac_tbl(256, 300);
    struct pollfd *pfds = new struct pollfd[2];
    RouteTable rtb;

    NetIf *netif[NETIF_NUM];
    netif[0] = new NetIf((char *)"h-ns11", IPAddress((char *)"172.18.0.1"), IPNetmask((char *)"255.255.255.0"));
    netif[1] = new NetIf((char *)"h-ns12", IPAddress((char *)"172.19.0.1"), IPNetmask((char *)"255.255.255.0"));

    for (int i = 0; i < NETIF_NUM; i++) {
        dump_netif_info(*netif[i]);
        IPNetwork net(netif[i]->getIP().touInt() & netif[i]->getIPNetmask().touInt(), netif[i]->getIPNetmask().getLength());
        rtb.addRoute(net, RouteType::RTYPE_CONNECTED, IPAddress("0.0.0.0"));

        pfds[i].fd = netif[i]->getFD();
        pfds[i].events = POLLIN | POLLERR;
    }

    Ethernet *pkt = new Ethernet();
    for (;;) {
        arp_tbl.refresh();
        mac_tbl.refresh();

        int poll_state = poll(pfds, NETIF_NUM, 1);
        if (poll_state == -1) {
            printf("polling error");
            continue;
        }
        for (int i = 0; i < NETIF_NUM; i++) {
            if (!(pfds[i].revents & (POLLIN | POLLERR))) {
                continue;
            }
            if (netif[i]->recv(pkt) == 0)
                continue;
            /*
            if ((pkt->getDst() != netif[i]->getMac()) && (pkt->getDst().toLong() != 0xffffffffffff))
                continue;
            */
            printf("\n\n--------------------------\n");
            printf("received interface:%s\n", netif[i]->getIfName());
            dlib::hexdump(pkt->RawData(), pkt->getLength());

            mac_tbl.update(pkt->getSrc(), netif[i]);
            //printf("ARP entry:%d\n", arp_tbl.getSize());
            //printf("MAC entry:%d\n", mac_tbl.getSize());

            if (pkt->isARP()) {
                if (pkt->arp().getOPCode() == ARPOPCode::ARP_REQUEST && pkt->arp().getDstIP() == netif[i]->getIP()) {
                    printf("[receive ARP request]\n");
                    printf("Who has %s ? [sender: %s(%" PRIx64 ")]\n", pkt->arp().getDstIP().toStr(),
                           pkt->arp().getSrcIP().toStr(), pkt->arp().getSrcMac().toLong());
                    reply_arp(*netif[i], *pkt);
                }

                arp_tbl.update(pkt->arp().getSrcIP(), pkt->arp().getSrcMac());
            }

            else if (pkt->isIPv4()) {
                printf("[IP length] %d\n", pkt->ipv4().getLength());
                printf("[IP] %s -> %s\n", pkt->ipv4().getSrc().toStr(), pkt->ipv4().getDst().toStr());

                Route _route = rtb.r_resolve(pkt->ipv4().getDst());
                switch (_route.getBestRouteType()) {
                    case RouteType::RTYPE_CONNECTED:
                        if (!arp_tbl.isExist(pkt->ipv4().getDst())) {
                            for (int j = 0; j < NETIF_NUM; j++) {
                                if (j == i) continue;
                                request_arp(*netif[j], *pkt);
                            }
                        } else {
                            //転送
                            MacAddress _mac = arp_tbl.get(pkt->ipv4().getDst());
                            if (mac_tbl.isExist(_mac)) {
                                NetIf *outif = mac_tbl.get(_mac);
                                pkt->setDst(_mac);
                                pkt->setSrc(outif->getMac());
                                printf("Send interface: %s\n", outif->getIfName());
                                outif->send(*pkt);
                            }
                        }
                        break;
                    case RouteType::RTYPE_UNREACH:
                        printf("Drop: Network Unreachable");
                        continue;
                        break;
                    default:
                        printf("Drop: Unsupported Route Type");
                        continue;
                        break;
                }
            }
        }
    }

    delete pkt;

    return (0);
}
