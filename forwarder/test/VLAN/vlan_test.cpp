#include <cstdio>
#include <cstdlib>
#include <poll.h>
#include <unistd.h>
#include <cinttypes>
#include <new>

#include <cstring>

#include "NetIf.hpp"
#include "MacAddress.hpp"
#include "Ethernet.hpp"
#include "HashMap.hpp"
#include "comlib.hpp"
#include "dlib.hpp"

#define OS_NTOHS ntohs
#define OS_HTONS htons

#define ETHERTYPE_PBB 0x88e7

#define IFMAX 4

/**
 * This code was written with reference to Lagopus switch
 * 
 * [Lagopus switch]
 * http://www.lagopus.org/
 *  - lagopus/src/dataplane/mgr/sock_io.c
 */
static ssize_t read_packet(int fd, uint8_t *buf, size_t buflen) {
    //struct sockaddr from;
    struct iovec iov;
    struct msghdr msg;
    union {
        struct cmsghdr cmsg;
        uint8_t buf[CMSG_SPACE(sizeof(struct tpacket_auxdata))];
    } cmsgbuf;
    struct cmsghdr *cmsg;
    struct tpacket_auxdata *auxdata;
    //uint16_t *p;
    ssize_t pktlen;
    //uint16_t ether_type;
    
    //uint8_t cmsgbuf[2048];
    
    iov.iov_base = buf;
    iov.iov_len = buflen;
    memset(buf, 0, buflen);
    
    //msg->msg_name = &from;
    msg.msg_name = NULL;
    //msg->msg_namelen = sizeof(from);
    msg.msg_namelen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = &cmsgbuf;
    msg.msg_controllen = sizeof(cmsgbuf);
    msg.msg_flags = 0;
    
    pktlen = recvmsg(fd, &msg, MSG_TRUNC);
    //printf("[recv]%dbyte\n", pktlen);
    
    
    printf("---\n", pktlen);
    dlib::hexdump((uint8_t *)&msg, 200);
    printf("---\n", pktlen);
    
    //dlib::hexdump((uint8_t *)&msg, pktlen);
    if (pktlen == -1) {
        if (errno == EAGAIN) {
            perror("recvmsg:");
            pktlen = 0;
        }
        return pktlen;
    }
    
    printf("cmg_type[%d]\n", ((cmsghdr *)&msg.msg_control)->cmsg_type);
    printf("control[%x]\n", &msg.msg_control);
    printf("addr[%x]\n", CMSG_FIRSTHDR(&msg));
    //for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
    for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
        if (cmsg->cmsg_type != PACKET_AUXDATA) {
            continue;
        }
        auxdata = (struct tpacket_auxdata *)CMSG_DATA(cmsg);
        //printf("--aux--\n");
        //dlib::hexdump((uint8_t *)auxdata, sizeof(tpacket_auxdata));
        //printf("--aux--\n");
        if ((auxdata->tp_status & TP_STATUS_VLAN_VALID) == 0) {
            printf("continue");
            continue;
        }
        if (auxdata->tp_vlan_tci == 0) {
            continue;
        }
        uint16_t *p = (uint16_t *)(buf+12);
        uint16_t ether_type;
        printf("%x\n", comlib::ntohs(p[0]));
        switch (comlib::ntohs(p[0])) {
            case ETHERTYPE_PBB:
            case ETHERTYPE_VLAN:
                ether_type = 0x88a8;
                break;
            default:
                ether_type = ETHERTYPE_VLAN;
                break;
        }
        memmove(&p[2], p, pktlen - ETHER_ADDR_LEN * 2);
        p[0] = comlib::htons(ether_type);
        p[1] = comlib::htons(auxdata->tp_vlan_tci);
        pktlen += 4;
    }
    //printf("[flg]%d\n", msg.msg_flags);
    
    return pktlen;
}


int main(int argc, char **argv){
    
    //perser
    Ethernet packet;
    
    NetIf netif("enp4s0", IFTYPE_L2_ACCESS, 1);
    
    //receive buffer
    uint8_t buf[2048];
    
    int buflen = 2048;
    
    //send
    /*
    uint8_t send_p[] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x81, 0x00, 0x0c, 0x1c,
        0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x34, 0x95, 0xdb, 0x2d, 0xb3, 0xf2,
        0xac, 0x12, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xac, 0x12, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    netif.sendRaw(send_p, sizeof(send_p));
    */
    while(true){
        //受信サイズ
        int s = read_packet(netif.getFD(), buf, buflen);
        dlib::hexdump(buf, s);
        Ethernet eth(buf, s);
        printf("%0.4x\n", eth.getType());
        printf("\n");
    }
    return(0);
}
