#ifndef INCLUDED_ROUTE_TABLE
#define INCLUDED_ROUTE_TABLE

#include <cinttypes>
#include <cstdint>

#include "Array.hpp"
#include "Exception.hpp"
#include "HashMap.hpp"
#include "IPAddress.hpp"

enum RouteType : uint8_t {
    RTYPE_LOCAL = 0,
    RTYPE_CONNECTED,
    RTYPE_STATIC,
    RTYPE_RIP,
    RTYPE_UNREACH,
    RTYPE_INVALID
};

using namespace sfwdr::Exception;

class Route {
  private:
    IPNetwork prefix;
    HashMap<RouteType, Array<IPAddress> *> nexthops;

    void _init();
    void _copy_from(const Route &route);

  public:
    Route(const IPNetwork &prefix);
    Route(char *prefix_str);
    Route(const Route &route);
    ~Route();

    Route &operator=(const Route &route);

    void addNexthop(RouteType type, IPAddress nexthop);

    Array<IPAddress> getNexthops(RouteType type) const;
    sfwdr::size_t getBestNexthops(Array<IPAddress> &ret) const;
    Array<IPAddress> getBestNexthops() const;
    RouteType getBestRouteType() const;
    sfwdr::size_t getRouteTypes(Array<RouteType> &ret) const;
    Array<RouteType> getRouteTypes() const;
    IPNetwork getNetwork() const;
};

struct PBIT {
    struct PBIT *n_pbit[2];
    struct Route *route;
};

class RouteTable {
  private:
    struct PBIT *root;

    void _init();
    void _r_delete(struct PBIT *pbit);

    struct PBIT *_initPBNode(struct PBIT *pbit);

    //void _r_getAllNetwork(Array<IPNetwork> &ret, struct PBIT *pbit);

  public:
    RouteTable();
    ~RouteTable();

    void addRoute(const IPNetwork &network, const RouteType type, const IPAddress &nexthop);
    Route getRoute(const IPNetwork &network) const;

    Route resolve(const IPAddress &addr) const;
    Route resolve(const IPNetwork &network) const;

    //再帰解決(経路種別による優先度あり)
    Route r_resolve(const IPAddress &addr) const;
    //sfwdr::size_t getAllNetwork(Array<IPNetwork> &ret);
};
#endif
