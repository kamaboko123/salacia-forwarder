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
    RouteType *keys = new RouteType[nexthops->getSize()];
    nexthops->getKeys(keys);
    for(int i = 0; i < nexthops->getSize(); i++){
        if(nexthops->get(keys[i]) != nullptr){
            delete nexthops->get(keys[i]);
        }
    }
    delete[] keys;
    
    delete prefix;
    delete nexthops;
}

void Route::addNexthop(RouteType type, IPAddress &nexthop){
    Array<IPAddress> *nhs = nexthops->get(type);
    if(nhs == nullptr){
        nhs = new Array<IPAddress>();
        nexthops->update(type, nhs);
    }
    nhs->add(nexthop);
}

Array<IPAddress> *Route::getNexthops(RouteType type){
    return(nexthops->get(type));
}

IPNetwork Route::getNetwork() const{
    return(*prefix);
}



RouteTable::RouteTable(){
    _init();
}

RouteTable::~RouteTable(){
    if(root.n_pbit[0] != nullptr){
        _r_delete(root.n_pbit[0]);
    }
    if(root.n_pbit[1] != nullptr){
        _r_delete(root.n_pbit[1]);
    }
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
    _initPBNode(&root);
}

struct PBIT *RouteTable::_initPBNode(struct PBIT *pbit){
    pbit->n_pbit[0] = nullptr;
    pbit->n_pbit[1] = nullptr;
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

sfwdr::size_t RouteTable::getAllNetwork(Array<IPNetwork> &ret){
    ret.clear();
    _r_getAllNetwork(ret, &root);
    return(ret.getSize());
}

void RouteTable::_r_getAllNetwork(Array<IPNetwork> &ret, struct PBIT *pbit){
    if(pbit->route != nullptr){
        ret.add(pbit->route->getNetwork());
    }
    for(int i = 0; i < 2; i++){
        if(pbit->n_pbit[i] != nullptr){
            _r_getAllNetwork(ret, pbit->n_pbit[i]);
        }
    }
}


