#include "RouteTable.hpp"

RouteTable::RouteTable(){
    init();
}

void RouteTable::init(){
    initPBNode(&root);
}

void RouteTable::initPBNode(struct PBIT *pbit){
    pbit->n_pbit[0] = nullptr;
    pbit->n_pbit[0] = nullptr;
    pbit->route.exist = false;
    pbit->route.prefix = 0;
    pbit->route.plen = 0;
    pbit->route.type = RTYPE_UNDEFINED;
}


