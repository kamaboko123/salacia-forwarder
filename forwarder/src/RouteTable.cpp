#include "RouteTable.hpp"

//RouteTypeのハッシュ関数を定義してHashMapで使えるようにする
template<>
int HashMap<RouteType, Array<IPAddress> *>::hash(RouteType rtype){ return(rtype % this->size); }

void Route::_init(){
    prefix = new IPNetwork("0.0.0.0/0");
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
    init();
}

void RouteTable::init(){
    initPBNode(&root);
}

void RouteTable::initPBNode(struct PBIT *pbit){
    pbit->n_pbit[0] = nullptr;
    pbit->n_pbit[0] = nullptr;
    pbit->route = new Route((char *)"0.0.0.0/0");
}

