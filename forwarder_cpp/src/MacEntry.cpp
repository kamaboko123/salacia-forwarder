#include "MacEntry.hpp"

MacEntry::MacEntry(NetIf *pif){
    this->pif = pif;
    updateTime();
}

void MacEntry::updateTime(){
    this->last_acc_t = time(NULL);
}

NetIf *MacEntry::getNetIf(){
    return(this->pif);
}

 uint64_t MacEntry::getTimeInt(){
    return(this->last_acc_t);
 }
