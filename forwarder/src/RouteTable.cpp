#include "RouteTable.hpp"

//RouteTypeのハッシュ関数を定義してHashMapで使えるようにする
template<>
int HashMap<RouteType, Array<IPAddress> *>::hash(RouteType rtype){ return(rtype % this->size); }

void Route::_init(){
    prefix = new IPNetwork();
    nexthops = new HashMap<RouteType, Array<IPAddress> *>(4);
}

Route::Route(IPNetwork prefix){
    _init();
    
    this->prefix->set(prefix);
}

Route::Route(char *prefix_str){
    _init();
    
    this->prefix->set(prefix_str);
}

Route::~Route(){
    delete prefix;
    delete nexthops;
}

void Route::addNexthop(RouteType type, IPAddress &nexthop){
    Array<IPAddress> *nhs = nexthops->get(type);
    if(nhs == nullptr){
        nhs = new Array<IPAddress>();
        nexthops->update(type, nhs);
    }
    IPAddress *nh = new IPAddress(nexthop);
    nhs->add(*nh);
}

Array<IPAddress> *Route::getNexthops(RouteType type){
    return(nexthops->get(type));
}




RouteTable::RouteTable(){
    _init();
}

void RouteTable::_init(){
    _initPBNode(&root);
}

struct PBIT *RouteTable::_initPBNode(struct PBIT *pbit){
    pbit->n_pbit[0] = nullptr;
    pbit->n_pbit[0] = nullptr;
    pbit->route = nullptr;
    return(pbit);
}

void RouteTable::addRoute(IPNetwork &network, RouteType type, IPAddress &nexthop){
    if(!network.isValid()) return;
    
    int bit;
    struct PBIT *p = &root;
    for(int i = 0; i < network.getNetmask().getLength(); i++){
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

Route *RouteTable::getRoute(IPNetwork &network){
    if(!network.isValid()) return(nullptr);
    
    int bit;
    struct PBIT *p = &root;
    for(int i = 0; i < network.getNetmask().getLength(); i++){
        bit = comlib::getBit(network.getNetaddr().touInt(), i);
        if(p->n_pbit[bit] == nullptr) return(nullptr);
        p = p->n_pbit[bit];
    }
    return(p->route);
}
