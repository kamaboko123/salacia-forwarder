#ifndef INCLUDED_CACHE_TABLE
#define INCLUDED_CACHE_TABLE

#include <ctime>
#include "HashMap.hpp"

template <typename V>
class CacheItem{
private:
    bool persistent;
    uint64_t last_ref;
    V item;
    
public:
    CacheItem<V>(bool persistent=false){
        V d_value;
        update(d_value, persistent);
    }
    CacheItem<V>(V value, bool persistent=false){
        update(value, persistent);
    }
    
    void update(V value, bool persistent=false){
        item = value;
        this->persistent = persistent;
        updateRefTime();
    }
    
    V getItem(){
        return item;
    }
    
    uint64_t getLastRef() const{
        return last_ref;
    }
    
    void updateRefTime(){
        last_ref = time(NULL);
    }
    
    bool isPersistent(){
        return persistent;
    }
};

template <typename K, typename V>
class CacheTable{
private:
    HashMap<K, CacheItem<V> *> *tbl;
    uint64_t age_out;
    
public:
    CacheTable<K, V>(uint32_t size = 256, uint64_t age_out=10){
        tbl = new HashMap<K, CacheItem<V> *>(size);
        this->age_out = age_out;
    }
    
    ~CacheTable(){
        refresh(true);
        delete tbl;
    }
    
    void update(K key, V value, bool persistent=false){
        if(tbl->isExist(key)){
            tbl->get(key)->update(value);
        }
        else{
            tbl->update(key, new CacheItem<V>(value, persistent));
        }
    }
    
    V get(K key){
        if(tbl->isExist(key)){
            return tbl->get(key)->getItem();
        }
        else{
            //return NULL;
            tbl->update(key, new CacheItem<V>());
            return get(key);
        }
    }
    
    //persisitent item will be deleted if delete_all is true
    void refresh(bool delete_all=false){
        uint64_t current = time(NULL);
        
        Array<K> *keys = new Array<K>;
        tbl->getKeys(*keys);
        
        for(sfwdr::size_t i = 0; i < keys->getSize(); i++){
            CacheItem<V> *item = tbl->get((*keys)[i]);
            if(delete_all){
                tbl->del((*keys)[i]);
                delete item;
            }
            else if(!item->isPersistent() && ((current - item->getLastRef()) > age_out)){
                tbl->del((*keys)[i]);
                delete item;
            }
        }
        
        delete keys;
    }
    
    uint32_t getSize() const{
        return tbl->getSize();
    }
    
    bool isExist(K key) const{
        return tbl->isExist(key);
    }
};

template <typename T>
class PointerWrapper{
private:
    T *p;
public:
    PointerWrapper(){
        p = nullptr;
    }
    PointerWrapper(T *p){
        this->p = p;
    }
    T *unwrap() const{
        return p;
    }
};


#endif
