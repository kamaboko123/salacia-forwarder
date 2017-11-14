#include <cstdint>

enum RouteType{
    RTYPE_LOCAL = 0,
    RTYPE_CONNECTED = 1,
    RTYPE_STATIC = 2,
    RTYPE_RIP = 3,
    RTYPE_UNDEFINED = 255
};

struct ROUTE{
    bool exist;
    uint32_t prefix;
    uint8_t plen;
    uint32_t nexthop;
    RouteType type;
};

struct PBIT{
    struct PBIT *n_pbit[2];
    struct ROUTE route;
};


class  RouteTable{
private:
    struct PBIT root;
    
    void init();
    void initPBNode(struct PBIT *pbit);
public:
    RouteTable();
    
};
