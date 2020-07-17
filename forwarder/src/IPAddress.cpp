#include "IPAddress.hpp"

void IPAddress::_init() {
    addr = 0;
    addr_str = new char[IP_ADDR_STR_LEN]();
}

void IPAddress::_free() {
    delete[] this->addr_str;
}

IPAddress::IPAddress() {
    _init();
    set((uint32_t)0);
}

IPAddress::IPAddress(uint32_t addr_uint) {
    _init();
    set(addr_uint);
}

IPAddress::IPAddress(const char *addr_str) {
    _init();
    try {
        set(addr_str);
    } catch (sfwdr::Exception::InvalidIPAddress &e) {
        _free();
        throw;
    }
}

IPAddress::~IPAddress() {
    _free();
}

IPAddress::IPAddress(const IPAddress &ipaddr) {
    _init();
    set(ipaddr);
}

IPAddress &IPAddress::operator=(const IPAddress &ipaddr) {
    if (this != &ipaddr) {
        set(ipaddr);
    }
    return (*this);
}

void IPAddress::set(uint32_t addr_uint) {
    addr = addr_uint;
    uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);
}

void IPAddress::set(const char *addr_str) {
    try {
        addr = iptoui(addr_str);
        uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);
    } catch (sfwdr::Exception::InvalidIPAddress &e) {
        addr = 0;
        uitoip(addr, this->addr_str, IP_ADDR_STR_LEN);

        throw e;
    }
}

void IPAddress::set(const IPAddress &ipaddr) {
    set(ipaddr.touInt());
}

uint32_t IPAddress::touInt() const {
    return (addr);
}

char *IPAddress::toStr() const {
    return (addr_str);
}

char *IPAddress::uitoip(uint32_t addr, char *retbuf, sfwdr::ssize_t retbuf_len) {
    if (retbuf_len < IP_ADDR_STR_LEN) return (nullptr);

    uint32_t ext_base = 0xFF000000;
    uint8_t ls = 24;
    ssize_t i = 0;
    char buf;

    //上位から1byteずつ抜き出して処理
    while (ext_base != 0) {
        //flg : hundred's place zero
        bool hp_zero = true;

        //100の位(0の場合は入れない)
        buf = (((addr & ext_base) >> ls) / 100) + '0';
        if (buf != '0') {
            hp_zero = false;
            retbuf[i++] = buf;
        }

        //10の位
        buf = (((addr & ext_base) >> ls) % 100 / 10) + '0';
        //10の位が0ではない
        // or
        //100の位が0じゃない
        if (buf != '0' || !hp_zero) retbuf[i++] = buf;

        //1の位(0でも入れる)
        retbuf[i++] = (((addr & ext_base) >> ls) % 10) + '0';

        //ドット入れる
        retbuf[i++] = '.';

        ext_base = ext_base >> 8;
        ls -= 8;
    }

    //最後のドットは不要なのでそのまま終端
    retbuf[--i] = '\0';

    return (retbuf);
}

uint32_t IPAddress::iptoui(char const *addr_str) {
    uint8_t octet = 0;
    uint32_t addr = 0;
    uint64_t tmp;
    const char *addr_str_org = addr_str;

    while (*addr_str != '\0') {
        tmp = comlib::atoi(addr_str);
        if (comlib::ndigit(tmp) > 3) {
            throw sfwdr::Exception::InvalidIPAddress(addr_str_org);
        }
        if (tmp > 255 || tmp < 0) {
            throw sfwdr::Exception::InvalidIPAddress(addr_str_org);
        }

        //1byte桁上げ + 桁の値加算
        addr = (addr << 8) + tmp;

        octet++;

        //次のオクテットまでポインタを進める
        do {
            addr_str++;
            if (*addr_str == '\0') break;
            if (*addr_str != '.' && (*addr_str < '0' || *addr_str > '9')) {
                //数値とドット以外はエラー
                throw sfwdr::Exception::InvalidIPAddress(addr_str_org);
            }
        } while (*(addr_str - 1) != '.');
    }
    if (octet != 4) {
        throw sfwdr::Exception::InvalidIPAddress(addr_str_org);
    }

    return (addr);
}

uint32_t IPAddress::getHash() const {
    return addr;
}

bool IPAddress::operator==(IPAddress v) {
    return (this->touInt() == v.touInt());
}

IPNetmask::IPNetmask() : IPAddress() {
    length = 0;
}

IPNetmask::IPNetmask(const char *addr_str) : IPAddress() {
    set(addr_str);
}

IPNetmask::IPNetmask(uint32_t addr_uint) : IPAddress() {
    set(addr_uint);
}

sfwdr::ssize_t IPNetmask::set(uint32_t addr_uint) {
    try {
        length = validIPNetmask(addr_uint);
        IPAddress::set(addr_uint);
    } catch (sfwdr::Exception::Exception &e) {
        char buf_ex[32];
        comlib::uitoa(addr_uint, buf_ex, 32);
        throw sfwdr::Exception::InvalidIPNetmask(buf_ex);
    }
    return (getLength());
}

sfwdr::ssize_t IPNetmask::set(const char *addr_str) {
    try {
        uint32_t addr_uint = iptoui(addr_str);
        set(addr_uint);
    } catch (sfwdr::Exception::Exception &e) {
        throw sfwdr::Exception::InvalidIPNetmask(addr_str);
    }
    return (getLength());
}

sfwdr::ssize_t IPNetmask::setLength(sfwdr::ssize_t mask_length) {
    if (mask_length == 0) return (set((uint32_t)0));
    if (mask_length < 0 || mask_length > 32) {
        char ex_buf[64] = "length ";
        char ex[32];

        comlib::strncat(ex_buf, comlib::uitoa(mask_length, ex, 32), 64);
        throw sfwdr::Exception::InvalidIPNetmask(ex_buf);
    }

    uint32_t mask = 0;
    uint8_t shift;
    for (int i = mask_length; i > 0; i--) {
        shift = i + 31 - mask_length;
        mask += (1 << shift);
    }
    set(mask);

    return (getLength());
}

sfwdr::size_t IPNetmask::validIPNetmask(uint32_t mask) {
    uint8_t length = 0;
    uint32_t cmask = 0x80000000;
    uint8_t buf;
    bool find_0 = false;

    for (int i = 32; i > 0; i--) {
        buf = (mask & cmask) >> (i - 1);

        if (find_0 && buf == 1) {
            char buf_ex[32];
            comlib::uitoa(mask, buf_ex, 32);

            throw sfwdr::Exception::InvalidIPNetmask(buf_ex);
        }
        if (buf == 0) find_0 = true;
        if (!find_0) length++;

        cmask = cmask >> 1;
    }

    return (length);
}

sfwdr::ssize_t IPNetmask::getLength() const {
    return (length);
}

IPNetwork::IPNetwork() {
    _init();
    _build_str();
}

IPNetwork::IPNetwork(const char *ipnet_str) {
    _init();
    try {
        set(ipnet_str);
    } catch (sfwdr::Exception::InvalidIPNetwork) {
        _free();
        throw;
    }
}

IPNetwork::IPNetwork(const IPAddress &ipaddr, sfwdr::ssize_t mask_length) {
    _init();
    try {
        set(ipaddr, mask_length);
    } catch (sfwdr::Exception::InvalidIPNetwork) {
        _free();
        throw;
    }
}

IPNetwork::~IPNetwork() {
    _free();
}

IPNetwork::IPNetwork(const IPNetwork &ipnet) {
    _init();
    set(ipnet);
}

IPNetwork &IPNetwork::operator=(const IPNetwork &ipnet) {
    if (this != &ipnet) {
        set(ipnet);
    }
    return (*this);
}

void IPNetwork::_init() {
    ipnw = new IPNW;
    prefix = new char[IP_PREFIX_STR_LEN]();
}

void IPNetwork::_free() {
    delete ipnw;
    delete[] prefix;
}

void IPNetwork::set(const char *nw_str) {
    try {
        *ipnw = validIPNetwork(nw_str);
        _build_str();
    } catch (sfwdr::Exception::InvalidIPNetwork) {
        throw;
    }
}

void IPNetwork::set(const IPAddress &ipaddr, sfwdr::ssize_t mask_length) {
    try {
        *ipnw = validIPNetwork(ipaddr, mask_length);
        _build_str();
    } catch (sfwdr::Exception::InvalidIPNetwork) {
        throw;
    }
}

void IPNetwork::set(const IPNetwork &ipnet) {
    (*ipnw).netaddr.set(ipnet.getNetaddr().touInt());
    (*ipnw).netmask.set(ipnet.getNetmask().touInt());
    _build_str();
}

const IPAddress &IPNetwork::getNetaddr() const {
    return ((*ipnw).netaddr);
}

const IPNetmask &IPNetwork::getNetmask() const {
    return ((*ipnw).netmask);
}

const char *IPNetwork::toStr() const {
    return (prefix);
}

IPNW IPNetwork::validIPNetwork(const char *nw_str) {
    char *nwaddr_str = nullptr;
    IPNW nw;

    try {
        //ネットワークアドレス
        sfwdr::ssize_t nwaddr_end_index;
        if ((nwaddr_end_index = comlib::strchr_index(nw_str, '/')) == -1) {
            //マスク無しはエラー
            throw sfwdr::Exception::InvalidIPNetwork("");
        }
        nwaddr_str = new char[nwaddr_end_index + 1]();
        comlib::strncat(nwaddr_str, nw_str, nwaddr_end_index);
        nw.netaddr.set(nwaddr_str);

        //サブネットマスク
        //残りの文字を数値変換->桁数カウント、残りの文字数と一致するか

        int prefix_len_str_len = comlib::strlen(&nw_str[nwaddr_end_index + 1]);
        int plen = comlib::atoi(&nw_str[nwaddr_end_index + 1]);
        if (prefix_len_str_len != comlib::ndigit(plen)) {
            throw sfwdr::Exception::InvalidIPNetwork("");
        }

        nw.netmask.setLength(plen);

        //ネットワークアドレスとサブネットマスクの論理積が、ネットワークアドレスと同じであれば正当
        if ((nw.netaddr.touInt() & nw.netmask.touInt()) != nw.netaddr.touInt()) {
            throw sfwdr::Exception::InvalidIPNetwork("");
        }

        delete[] nwaddr_str;
    } catch (sfwdr::Exception::Exception) {
        delete[] nwaddr_str;
        throw sfwdr::Exception::InvalidIPNetwork(nw_str);
    }

    return (nw);
}

IPNW IPNetwork::validIPNetwork(const IPAddress &ipaddr, sfwdr::ssize_t mask_length) {
    IPNW nw;
    try {
        nw.netaddr = ipaddr;
        nw.netmask.setLength(mask_length);

        if ((nw.netaddr.touInt() & nw.netmask.touInt()) != nw.netaddr.touInt()) {
            throw sfwdr::Exception::InvalidIPNetwork("");
        }
    } catch (sfwdr::Exception::Exception) {
        //あとでなおす
        throw sfwdr::Exception::InvalidIPNetwork(ipaddr.toStr());
    }

    return (nw);
}

void IPNetwork::_build_str() {
    int len;
    comlib::memset((uint8_t *)prefix, '\0', IP_PREFIX_STR_LEN);
    comlib::strncat(prefix, (*ipnw).netaddr.toStr(), IP_PREFIX_STR_LEN);
    len = comlib::strlen(prefix);
    comlib::strncat(prefix, "/", IP_PREFIX_STR_LEN - len);
    len++;

    char buf[4];
    comlib::uitoa((*ipnw).netmask.getLength(), buf, 4);
    comlib::strncat(prefix, buf, IP_PREFIX_STR_LEN - len);
}
