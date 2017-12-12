#include "NetIf.hpp"

NetIf::NetIf(){
    return;
}

NetIf::NetIf(char *ifname, IfType iftype, uint16_t vlan){
    comlib::strncpy(this->ifname, ifname, sizeof(this->ifname));
    
    this->iftype = iftype;
    this->vlan = vlan;
    
    initIf();
    /*
    switch(this->iftype){
        case IFTYPE_L2_ACCESS:
            initIf(vlan);
            break;
        case IFTYPE_L2_TRUNK:
            initIf(vlan);
            break;
        default:
            break;
    }*/
}

NetIf::NetIf(char *ifname, IPAddress addr, IPNetmask mask){
    comlib::strncpy(this->ifname, ifname, sizeof(this->ifname));
    this->iftype = IFTYPE_L3_V4;
    this->vlan = 0;
    
    ip_addr = addr;
    ip_mask = mask;
    
    initIf();
}

NetIf::~NetIf(){
    close(this->pd);
}

void NetIf::initIf(){
    //socket作る
    this->pd = -1;
    if((this->pd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1){
        perror("socket()");
        exit(1);
    }
    
    //option
    int on = 1;
    if (setsockopt(this->pd, SOL_PACKET, PACKET_AUXDATA, &on, sizeof(on)) == -1){
        perror("bind():");
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
    //fcntl(this->pd, F_SETFL, O_NONBLOCK);
    ioctl(this->pd, FIONBIO, &on);
    
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

IfType NetIf::getIfType(){
    return(this->iftype);
}

char *NetIf::getIfName(){
    return(this->ifname);
}

int NetIf::getFD(){
    return(this->pd);
}

/*
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
*/
int NetIf::sendRaw(uint8_t *data, int length){
    return(write(this->pd, data, length));
}

int NetIf::send(Ethernet packet, uint16_t vlan){
    int ret;
    switch(this->iftype){
        case IFTYPE_L2_ACCESS:
            packet.removeVlanTag();
            ret = sendRaw(packet.RawData(), packet.getLength());
            break;
        case IFTYPE_L2_TRUNK:
            packet.setVlanTag(vlan);
            ret = sendRaw(packet.RawData(), packet.getLength());
            break;
        default:
            ret = 0;
            break;
    }
    return(ret);
}

uint16_t NetIf::recv(Ethernet *eth){
    uint8_t buf[2048];
    uint16_t s = recvRaw(buf, sizeof(buf));
    if(s == 0){
        return(0);
    }
    eth->set(buf, s);
    return(eth->getLength());
}

uint16_t NetIf::recvRaw(uint8_t *buf, uint16_t buflen){
    struct iovec iov;
    struct msghdr msg;
    union {
        struct cmsghdr cmsg;
        uint8_t buf[CMSG_SPACE(sizeof(struct tpacket_auxdata))];
    } cmsgbuf;
    struct cmsghdr *cmsg;
    struct tpacket_auxdata *auxdata;
    int pktlen = 0;
    
    iov.iov_base = buf;
    iov.iov_len = buflen;
    comlib::memset(buf, 0, buflen);
    
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = &cmsgbuf;
    msg.msg_controllen = sizeof(cmsgbuf);
    msg.msg_flags = 0;
    
    pktlen = recvmsg(this->pd, &msg, (MSG_TRUNC | MSG_DONTWAIT));
    
    if (pktlen == -1) {
        pktlen = 0;
        if (errno == EAGAIN) {
            //perror("recvmsg:");
        }
        return(pktlen);
    }
    
    //printf("cmg_type[%d]\n", ((cmsghdr *)&msg.msg_control)->cmsg_type);
    //printf("control[%x]\n", &msg.msg_control);
    //printf("addr[%x]\n", CMSG_FIRSTHDR(&msg));
    
    //補助メッセージを読んでいく(VLANとか入ってる)
    for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
        //AUX_DATAじゃなければスキップ
        if (cmsg->cmsg_type != PACKET_AUXDATA) {
            continue;
        }
        
        //AUX_DATAを読み取る
        auxdata = (struct tpacket_auxdata *)CMSG_DATA(cmsg);
        
        //VLAN ID持ってないならスキップ
        //ステータスとTCIで判定
        if ((auxdata->tp_status & TP_STATUS_VLAN_VALID) == 0) {
            continue;
        }
        if (auxdata->tp_vlan_tci == 0) {
            continue;
        }
        
        //VLAN処理(frameにtag挿入)
        //raw socketで受信したフレームのEth Typeを調べる
        //raw socketで受けたフレームは802.1Qタグが外れているので、中のtypeが取得できる
        //not support(802.1AH)
        
        //buf(受信フレームの頭)から12byte目(6 * 2)から見る
        uint16_t *p = (uint16_t *)(buf + MAC_ADDR_SIZE * 2);
        uint16_t ether_type;
        
        switch (comlib::ntohs(p[0])) {
            case ETHTYPE_DOT1Q:
                //中のtyoeが802.1Qなら外側のtypeはQinQ(802.1d)の規定に従う
                //未検証(いつか動作確認する)
                ether_type = 0x88a8;
                break;
            default:
                //外側のtypeに802.1Qを挿入
                ether_type = ETHTYPE_DOT1Q;
                break;
        }
        
        //802.1Qタグを挿入するために4byte後ろにずらす
        comlib::memmove((uint8_t *)&p[2], (uint8_t *)p, pktlen - (MAC_ADDR_SIZE * 2));
        
        //4byteずらして空けたスペースに802.1Qタグを突っ込む
        //Ethtype(前2byte)
        p[0] = comlib::htons(ether_type);
        //TCI(後ろ2bytem、後ろ12byteがVLAN ID)
        p[1] = comlib::htons(auxdata->tp_vlan_tci);
        
        //4byte伸ばす
        pktlen += DOT1Q_TAG_SIZE;
        /*
        memmove(&p[2], p, pktlen - MAC_ADDR_SIZE * 2);
        p[0] = comlib::htons(ether_type);
        p[1] = comlib::htons(auxdata->tp_vlan_tci);
        pktlen += 4;
        */
    }
    
    return pktlen;
}

uint16_t NetIf::getVlanId(){
    return(vlan);
}
