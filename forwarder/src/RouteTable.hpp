#ifndef INCLUDED_ROUTE_TABLE
#define INCLUDED_ROUTE_TABLE

#include <cstdint>
#include <cinttypes>
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
    
    IPNetwork getNetwork() const;
};

struct PBIT{
    struct PBIT *n_pbit[2];
    struct Route *route;
};

class  RouteTable{
private:
    struct PBIT root;
    
    void _init();
    void _r_delete(struct PBIT *pbit);
    
    struct PBIT *_initPBNode(struct PBIT *pbit);
    
    void _r_getAllNetwork(Array<IPNetwork> &ret, struct PBIT *pbit);
public:
    RouteTable();
    ~RouteTable();
    
    void addRoute(IPNetwork &network, RouteType type, IPAddress &nexthop);
    Route *getRoute(IPNetwork &network);
    
    sfwdr::size_t getAllNetwork(Array<IPNetwork> &ret);
};

#endif
