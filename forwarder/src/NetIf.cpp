#include "NetIf.hpp"

NetIf::NetIf(){
    return;
}

NetIf::NetIf(char *ifname, IfType iftype, uint16_t vlan){
    comlib::strncpy(this->ifname, ifname, sizeof(this->ifname));
    
    this->iftype = iftype;
    switch(iftype){
        case IFTYPE_L2_ACCESS:
            initL2aIf(vlan);
            break;
        case IFTYPE_L2_TRUNK:
            break;
        default:
            break;
    }
}

void NetIf::initL2aIf(uint16_t vlan){
    //socket作る
    this->pd = -1;
    if((this->pd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1){
        perror("socket()");
        exit(1);
    }
    
    //interfaceの名前からifindexを取ってくる
    this->ifr = {0};
    comlib::strncpy(this->ifr.ifr_name, this->ifname, IFNAMSIZ);
    if (ioctl(this->pd, SIOCGIFINDEX, &(this->ifr)) == -1) {
        perror("SIOCGIFINDEX");
        exit(1);
    }
    this->ifindex = this->ifr.ifr_ifindex;
    
    //HWADDR取得
    this->ifr = {0};
    comlib::strncpy(this->ifr.ifr_name, this->ifname, IFNAMSIZ);
    if(ioctl(this->pd, SIOCGIFHWADDR, &(this->ifr)) == -1){
        perror("SIOCGHIFWADDR");
        exit(1);
    }
    this->myaddr = this->ifr.ifr_hwaddr;
    
    this->sll = {0};
    
    //socketにinterfaceをbind
    this->sll.sll_family = AF_PACKET; //allways AF_PACKET
    this->sll.sll_protocol = htons(ETH_P_ALL);
    this->sll.sll_ifindex = this->ifindex;
   
    if (bind(this->pd, (struct sockaddr *)&(this->sll), sizeof(this->sll)) == -1) {
        perror("bind():");
        exit(1);
    }
   
    //ノンブロッキングモードに
    //ioctl(this->pd, FIONBIO, 1);
    
    //config
    this->iftype = IFTYPE_L2_ACCESS;
    this->vlan = vlan;
    
    
    //取ってきたMacAddressを扱いやすいようにする
    uint64_t mac_int = 0;
    for(int i = 0; i < 6; i++){
        mac_int += (((uint64_t)this->myaddr.sa_data[i]) << ((6 - 1 - i)*8));
    }
    
    this->mac_addr = mac_int;
}

MacAddress NetIf::getMac(){
    return(this->mac_addr);
}

int NetIf::send(uint8_t *data, int length, uint16_t vlan){
    // return
    // -1 : error
    
    int ret;
    
    switch(this->iftype){
        case IFTYPE_L2_ACCESS:
            ret = sendL2a(data, length);
            break;
        default:
            return(0);
    }
    
    return(ret);
}

int NetIf::sendL2a(uint8_t *data, int length){
    return(sendRaw(data, length));
}

int NetIf::sendRaw(uint8_t *data, int length){
    return(write(this->pd, data, length));
}
