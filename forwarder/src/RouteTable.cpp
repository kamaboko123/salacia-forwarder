#include "RouteTable.hpp"

//RouteTypeのハッシュ関数を定義してHashMapで使えるようにする
template<>
int HashMap<RouteType, Array<IPAddress> *>::hash(RouteType rtype) const{ return(rtype % this->size); }

void Route::_init(){
    return;
}

Route::Route(const IPNetwork &prefix){
    _init();
    this->prefix.set(prefix);
    if(!prefix.isValid()) throw Exception((char *)"Route :invalid Network Address");
}

Route::Route(char *prefix_str){
    _init();
    this->prefix.set(prefix_str);
    if(!prefix.isValid()) throw Exception((char *)"Route :invalid Network Address");
}

Route::Route(const Route &route){
    _init();
    _copy_from(route);
}

Route &Route::operator=(const Route &route){
    if(this != &route){
        _copy_from(route);
    }
    return(*this);
}

Route::~Route(){
    Array<RouteType> types;
    nexthops.getKeys(types);
    for(sfwdr::size_t i = 0; i < types.getSize(); i++){
        delete nexthops.get(types.get(i));
    }
    return;
}

void Route::_copy_from(const Route &route){
    prefix = route.getNetwork();
    
    Array<RouteType> keys;
    nexthops.getKeys(keys);
    for(sfwdr::size_t i = 0; i < keys.getSize(); i++){
        delete nexthops.get(keys.get(i));
    }
    nexthops.clear();
    
    Array<RouteType> types;
    route.getRouteTypes(types);
    for(sfwdr::size_t i = 0; i < types.getSize(); i++){
        Array<IPAddress> nhs = route.getNexthops(types.get(i));
        for(sfwdr::size_t j = 0; j < nhs.getSize(); j++){
            addNexthop(types.get(i), nhs.get(j));
        }
    }
}

void Route::addNexthop(RouteType type, const IPAddress nexthop){
    Array<IPAddress> *nhs = nexthops.get(type);
    if(nhs == nullptr){
        nhs = new Array<IPAddress>();
        nexthops.update(type, nhs);
    }
    nhs->add(nexthop);
}

Array<IPAddress> Route::getNexthops(RouteType type) const{
    Array<IPAddress> *ret = nexthops.get(type);
    if(ret != nullptr) return(*ret);
    return(Array<IPAddress>());
}

IPNetwork Route::getNetwork() const{
    return(prefix);
}

sfwdr::size_t Route::getRouteTypes(Array<RouteType> &ret) const{
    return(nexthops.getKeys(ret));
}

Array<RouteType> Route::getRouteTypes() const{
    Array<RouteType> ret;
    getRouteTypes(ret);
    return(ret);
}

RouteType Route::getBestRouteType() const{
    for(uint8_t i = RTYPE_LOCAL; i < RTYPE_INVALID; i++){
        if(nexthops.isExist((RouteType)i)) return((RouteType)i);
    }
    return(RTYPE_INVALID);
}

sfwdr::size_t Route::getBestNexthops(Array<IPAddress> &ret) const{
    ret = getBestNexthops();
    return(ret.getSize());
}

Array<IPAddress> Route::getBestNexthops() const{
    Array<IPAddress> ret;
    
    RouteType best_type = getBestRouteType();
    if(best_type != RTYPE_INVALID){
        ret = getNexthops(best_type);
    }
    
    return(ret);
}





RouteTable::RouteTable(){
    _init();
}

RouteTable::~RouteTable(){
    _r_delete(root);
}

void RouteTable::_r_delete(struct PBIT *pbit){
    if(pbit == nullptr) return;
    if(pbit->n_pbit[0] != nullptr){
        _r_delete(pbit->n_pbit[0]);
    }
    if(pbit->n_pbit[1] != nullptr){
        _r_delete(pbit->n_pbit[1]);
    }
    if(pbit->route != nullptr){
        delete pbit->route;
    }
    delete pbit;
}

void RouteTable::_init(){
    root = _initPBNode(new struct PBIT());
}

struct PBIT *RouteTable::_initPBNode(struct PBIT *pbit){
    pbit->n_pbit[0] = nullptr;
    pbit->n_pbit[1] = nullptr;
    pbit->route = nullptr;
    return(pbit);
}

void RouteTable::addRoute(const IPNetwork &network, const RouteType type, const IPAddress &nexthop){
    if(!network.isValid()) return;
    
    int bit;
    struct PBIT *p = root;
    for(int i = 31; i > (31 - network.getNetmask().getLength()); i--){
        bit = comlib::getBit(network.getNetaddr().touInt(), i);
        if(p->n_pbit[bit] == nullptr){
            p->n_pbit[bit] = _initPBNode(new struct PBIT());
        }
        p = p->n_pbit[bit];
    }
    
    if(p->route == nullptr){
        p->route = new Route(network);
    }
    p->route->addNexthop(type, nexthop);
}

Route RouteTable::getRoute(const IPNetwork &network) const{
    if(!network.isValid()) return(nullptr);
    
    int bit;
    PBIT *p = root;
    for(int i = 31; i > (31 - network.getNetmask().getLength()); i--){
        bit = comlib::getBit(network.getNetaddr().touInt(), i);
        if(p->n_pbit[bit] == nullptr) return(nullptr);
        p = p->n_pbit[bit];
    }
    return(*(p->route));
}

Route RouteTable::resolve(const IPAddress &addr) const{
    IPNetwork tmp(addr, 32);
    return(resolve(tmp));
}

Route RouteTable::resolve(const IPNetwork &network) const{
    int bit;
    PBIT *p = root;
    Route ret((char *)"0.0.0.0/0");
    
    for(int i = 31; i > (31 - network.getNetmask().getLength()); i--){
        if(p->route != nullptr) ret = *(p->route);
        bit = comlib::getBit(network.getNetaddr().touInt(), i);
        if(p->n_pbit[bit] == nullptr) break;
        p = p->n_pbit[bit];
    }
    
    if(ret.getNetwork().getNetaddr().touInt() == 0){
        if(root->route != nullptr){
            ret = *(root->route);
        }
    }
    
    return(ret);
}

Route RouteTable::r_resolve(const IPAddress &addr) const{
    Route r = resolve(addr);
    RouteType br_type = r.getBestRouteType();
    
    while(br_type != RTYPE_CONNECTED){
        if(r.getBestNexthops().getSize() == 0){
            IPNetwork d_r = IPNetwork((char *)"0.0.0.0/0");
            return(resolve(d_r));
        }
        
        r = resolve(r.getBestNexthops().get(0));
        br_type = r.getBestRouteType();
    }
    
    return(r);
}
