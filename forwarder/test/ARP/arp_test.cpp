#include<cinttypes>
#include "../../src/Ethernet.hpp"
#include "../../src/comlib.hpp"
#include "../../src/dlib.hpp"
#include "../../src/NetIf.hpp"

int main(void){
    uint8_t arp_base[] = PKT_ARP;
    
    try{
        Ethernet eth1(arp_base, PKT_ARP_SIZE);
        printf("[type] : %.4x\n", eth1.getULType());
        printf("[src]%012" PRIx64 "\n", eth1.getSrc().toLong());
        printf("[dst]%012" PRIx64 "\n", eth1.getDst().toLong());
        
        eth1.arp();
        printf("[arp-opcode ]%d\n", eth1.arp().getOPCode());
        printf("[arp-src-mac]%012" PRIx64 "\n", eth1.arp().getSrcMac().toLong());
        printf("[arp-dst-mac]%012" PRIx64 "\n", eth1.arp().getDstMac().toLong());
        printf("[arp-src-ip ]%s\n", eth1.arp().getSrcIP().toStr());
        printf("[arp-dst-ip ]%s\n", eth1.arp().getDstIP().toStr());
        
        printf("\n");
        
        eth1.arp().setOPCode(RARP_REQUEST);
        printf("[arp-opcode ]%d\n", eth1.arp().getOPCode());
        
        MacAddress new_src_mac((char *)"77:88:99:AA:BB:CC");
        eth1.arp().setSrcMac(new_src_mac);
        printf("[arp-src-mac]%012" PRIx64 "\n", eth1.arp().getSrcMac().toLong());
        
        MacAddress new_dst_mac((char *)"11:22:33:44:55:66");
        eth1.arp().setDstMac(new_dst_mac);
        printf("[arp-dst-mac]%012" PRIx64 "\n", eth1.arp().getDstMac().toLong());
        
        IPAddress new_src_ip((char *)"1.1.1.1");
        eth1.arp().setSrcIP(new_src_ip);
        printf("[arp-src-ip ]%s\n", eth1.arp().getSrcIP().toStr());
        
        IPAddress new_dst_ip((char *)"2.2.2.2");
        eth1.arp().setDstIP(new_dst_ip);
        printf("[arp-dst-ip ]%s\n", eth1.arp().getDstIP().toStr());
    }
    catch(sfwdr::Exception::Exception &e){
        printf("[Exception]%s\n", e.getMessage());
    }
    
    //send
    IPAddress dst_ip("172.17.0.3");
    IPAddress src_ip("172.17.0.200");
    IPNetmask src_ip_mask("255.255.255.0");
    MacAddress src_mac("00:3d:2c:11:11:11");
    
    NetIf nif("veth767a7ac", src_ip, src_ip_mask);
    
    Ethernet eth2(arp_base, PKT_ARP_SIZE);
    eth2.arp().setOPCode(ARP_REQUEST);
    
    eth2.arp().setDstIP(dst_ip);
    eth2.arp().setSrcIP(src_ip);
    eth2.arp().setSrcMac(src_mac);
    //eth2.setSrc(src_mac);
    
    printf("send : %d byte\n", nif.sendBroadcast(eth2));
    
    return(0);
}
