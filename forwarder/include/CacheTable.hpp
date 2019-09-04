#ifndef INCLUDED_CACHE_TABLE
#define INCLUDED_CACHE_TABLE

#include <ctime>
#include "HashMap.hpp"

template <typename V>
class CacheItem{
private:
    uint64_t last_ref;
    V item;
    
public:
    CacheItem<V>(){
        V d_value;
        update(d_value);
    }
    CacheItem<V>(V value){
        update(value);
    }
    
    void update(V value){
        item = value;
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
        delete tbl;
    }
    
    void update(K key, V value){
        if(tbl->isExist(key)){
            tbl->get(key)->update(value);
        }
        else{
            tbl->update(key, new CacheItem<V>(value));
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
    
    void refresh(){
        uint64_t current = time(NULL);
        
        Array<K> *keys = new Array<K>;
        tbl->getKeys(*keys);
        
        for(sfwdr::size_t i = 0; i < keys->getSize(); i++){
            CacheItem<V> *item = tbl->get((*keys)[i]);
            if((current - item->getLastRef()) > age_out){
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
