#ifndef INCLUDED_HASH_MAP
#define INCLUDED_HASH_MAP

#include <iostream>
#include "Array.hpp"
#include "comlib.hpp"

template <typename K, typename V>
class HashMapItem{
private:
    bool empty;
    K key;
    V obj;
    HashMapItem<K, V> *next;
    HashMapItem<K, V> *prev;
    
public:
    HashMapItem<K, V>(){
        init();
    }
    
    void init(){
        //std::cout << "init" << std::endl;
        this->empty = true;
        this->next = nullptr;
        this->prev = nullptr;
    }
    
    void set(K k, V v,  HashMapItem<K, V> *next, HashMapItem<K, V> *prev){
        this->empty = false;
        this->key = k;
        this->obj = v;
        this->next = next;
        this->prev = prev;
    }
    
    void setValue(V v){
        this->obj = v;
    }
    
    void setNext(HashMapItem<K, V> *next){
        this->next = next;
    }
    
    HashMapItem<K, V> *getNext(){
        return(this->next);
    }
    
    void setPrev(HashMapItem<K, V> *prev){
        this->prev = prev;
    }
    HashMapItem<K, V> *getPrev(){
        return(this->prev);
    }
    
    K getKey(){
        return(this->key);
    }
    
    V get(){
        return(this->obj);
    }
    
    bool isEmpty(){
        return(this->empty);
    }
};

template <typename K, typename V>
class HashMap{
private:
    HashMapItem<K, V> *tbl;
    V *d_value;
    int size;
    
    void _init(int size){
        this->size = size;
        this->tbl = new HashMapItem<K, V>[this->size];
        this->d_value = new V();
    }
    
    HashMapItem<K, V> *_getItem(K key) const{
        int index = hash(key);
        if(tbl[index].isEmpty()) return(nullptr);
        
        if(tbl[index].getKey() == key){
            return(&tbl[index]);
        }
        
        HashMapItem<K, V> *p = &tbl[index];
        if((p = p->getNext()) == nullptr) return(nullptr);
        do{
            if(p->getKey() == key){
                return(p);
            }
        }while((p = p->getNext()) != nullptr);
        
        return(nullptr);
    }
    
public:
    HashMap<K, V>(int size = 256){
        _init(size);
    }
    
    ~HashMap(){
        clear();
        delete[] this->tbl;
        delete d_value;
    }
    
    void clear(){
        Array<K> *keys = new Array<K>();
        int size = getKeys(*keys);
        
        for(int i = 0; i < size; i++){
            del(keys->get(i));
        }
        
        delete keys;
    }
    
    void update(K key, V value){
        
        int index = hash(key);
        if(tbl[index].isEmpty()){
            //fist node is empty
            tbl[index].set(key, value, nullptr, nullptr);
            return;
        }
        
        HashMapItem<K, V> *p;
        p = &(tbl[index]);
        while(p->getNext() != nullptr){
            //keyが同じなら中身更新
            if(p->getKey() == key){
                p->setValue(value);
                return;
            }
            p = p->getNext();
        }
        
        //last node
        //keyが同じなら中身更新
        if(p->getKey() == key){
            p->setValue(value);
            return;
        }
        
        //create new node
        HashMapItem<K, V> *ne = new HashMapItem<K, V>();
        p->setNext(ne);
        ne->set(key, value, nullptr, p);
    }
    
    V get(K key) const{
        HashMapItem<K, V> *result = _getItem(key);
        if(result == nullptr){
            throw sfwdr::Exception::OutOfRange();
        }
        return(result->get());
    }
    
    V &operator [](const K &key) const{
        return(get(key));
    }
    
    bool isExist(K key) const{
        if(_getItem(key) != nullptr) return(true);
        return(false);
    }
    
    int getSize(){
        int ret = 0;
        for(int i = 0; i < size; i++){
            HashMapItem<K, V> *p = &tbl[i];
            if(p->isEmpty()) continue;
            ret++;
            while(p->getNext() != nullptr){
                ret++;
                p = p->getNext();
            }
        }
        return(ret);
    }
    
    void _dump(){
        for(int i = 0; i < size; i++){
            HashMapItem<K, V> *p = &tbl[i];
            if(p->isEmpty()) continue;
            printf("!");
            while(p->getNext() != nullptr){
                printf("!");
                p = p->getNext();
            }
            printf("\n");
        }
    }
    
    [[deprecated("please use getKeys(Array<K> &) function")]]
    K *getKeys(){
        K *ret = new K[getSize()];
        int j = 0;
        for(int i = 0; i < size; i++){
            HashMapItem<K, V> *p = &tbl[i];
            if(p->isEmpty()) continue;
            ret[j] = tbl[i].getKey();
            j++;
            while(p->getNext() != nullptr){
                ret[j] = p->getNext()->getKey();
                j++;
                p = p->getNext();
            }
        }
        return(ret);
    }
    
    [[deprecated("please use getKeys(Array<K> &) function")]]
    K *getKeys(K *ret){
        int j = 0;
        for(int i = 0; i < size; i++){
            HashMapItem<K, V> *p = &tbl[i];
            if(p->isEmpty()) continue;
            ret[j] = tbl[i].getKey();
            j++;
            while(p->getNext() != nullptr){
                ret[j] = p->getNext()->getKey();
                j++;
                p = p->getNext();
            }
        }
        return(ret);
    }
    
    sfwdr::size_t getKeys(Array<K> &ret) const{
        ret.clear();
        for(int i = 0; i < size; i++){
            HashMapItem<K, V> *p = &tbl[i];
            if(p->isEmpty()) continue;
            ret.add(tbl[i].getKey());
            while(p->getNext() != nullptr){
                ret.add(p->getNext()->getKey());
                p = p->getNext();
            }
        }
        
        return(ret.getSize());
    }
    
    bool del(K key){
        HashMapItem<K, V> *target = _getItem(key);
        if(target == nullptr) return(false);
    
        if(target->getPrev() == nullptr){
            //first node
            if(target->getNext() == nullptr){
                //first node && next node is not exist
                target->init();
                return(true);
            }
            else{
                //first node && next node is exist
                HashMapItem<K, V> *se = target->getNext();
                target->set(se->getKey(), se->get(), se->getNext(), nullptr);
                delete se;
                return(true);
            }
            
        }
        
        //not first node
        if(target->getNext() == nullptr){
            //last node
            target->getPrev()->setNext(nullptr);
            delete target;
            return(true);
        }
        
        //middle node
        target->getPrev()->setNext(target->getNext());
        target->getNext()->setPrev(target->getPrev());
        delete target;
        return(true);
    }
    
    int hash(K key) const{
        return(key.getHash() % this->size);
    }
    
};

#endif
