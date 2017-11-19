#ifndef INCLUDED_ROUTE_TABLE
#define INCLUDED_ROUTE_TABLE

#include <cstdint>
#include "IPAddress.hpp"
#include "HashMap.hpp"
#include "Array.hpp"

enum RouteType{
    RTYPE_LOCAL = 0,
    RTYPE_CONNECTED = 1,
    RTYPE_STATIC = 2,
    RTYPE_RIP = 3,
    RTYPE_UNDEFINED = 255
};

class Route{
private:
    IPNetwork *prefix;
    HashMap<RouteType, Array<IPAddress> *> *nexthops;
    
    void _init();
public:
    Route(IPNetwork prefix);
    Route(char *prefix_str);
    ~Route();
    
    void addNexthop(RouteType type, IPAddress &nexthop);
    
    Array<IPAddress> *getNexthops(RouteType type);
};

struct PBIT{
    struct PBIT *n_pbit[2];
    struct Route *route;
};

class  RouteTable{
private:
    struct PBIT root;
    
    void init();
    void initPBNode(struct PBIT *pbit);
public:
    RouteTable();
    
};

#endif
