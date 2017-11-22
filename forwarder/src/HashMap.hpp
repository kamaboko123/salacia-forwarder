#ifndef INCLUDED_HASH_MAP
#define INCLUDED_HASH_MAP

#include <iostream>
#include "Array.hpp"
#include "comlib.hpp"

template <typename K, typename V>
class HashMapEntry{
private:
    bool empty;
    K key;
    V obj;
    HashMapEntry<K, V> *next;
    HashMapEntry<K, V> *prev;
    
public:
    HashMapEntry<K, V>(){
        init();
    }
    
    void init(){
        //std::cout << "init" << std::endl;
        this->empty = true;
        this->next = nullptr;
        this->prev = nullptr;
    }
    
    void set(K k, V v,  HashMapEntry<K, V> *next, HashMapEntry<K, V> *prev){
        this->empty = false;
        this->key = k;
        this->obj = v;
        this->next = next;
        this->prev = prev;
    }
    
    void setValue(V v){
        this->obj = v;
    }
    
    void setNext(HashMapEntry<K, V> *next){
        this->next = next;
    }
    
    HashMapEntry<K, V> *getNext(){
        return(this->next);
    }
    
    void setPrev(HashMapEntry<K, V> *prev){
        this->prev = prev;
    }
    HashMapEntry<K, V> *getPrev(){
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
    HashMapEntry<K, V> *tbl;
    V *d_value;
    int size;
    
    void _init(int size){
        this->size = size;
        this->tbl = new HashMapEntry<K, V>[this->size];
        this->d_value = new V;
    }
    
    HashMapEntry<K, V> *_getEntry(K key){
        int index = hash(key);
        if(tbl[index].isEmpty()) return(nullptr);
        
        if(tbl[index].getKey() == key){
            return(&tbl[index]);
        }
        
        HashMapEntry<K, V> *p = &tbl[index];
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
        //std::cout << "destructer HashMap" << std::endl;
        
        int size = getSize();
        K *keys = new K[size];
        getKeys(keys);
        
        for(int i = 0; i < size; i++){
            //std::cout << "delete : ";
            //std::cout << keys[i].toInt() << std::endl;
            del(keys[i]);
        }
        
        delete[] keys;
        delete[] this->tbl;
        delete d_value;
    }
    
    void update(K key, V value){
        
        int index = hash(key);
        if(tbl[index].isEmpty()){
            //fist node is empty
            tbl[index].set(key, value, nullptr, nullptr);
            return;
        }
        
        HashMapEntry<K, V> *p;
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
        HashMapEntry<K, V> *ne = new HashMapEntry<K, V>();
        p->setNext(ne);
        ne->set(key, value, nullptr, p);
    }
    
    V get(K key){
        HashMapEntry<K, V> *result = _getEntry(key);
        if(result == nullptr) return(*d_value);
        return(result->get());
    }
    
    bool isExist(K key){
        if(_getEntry(key) != nullptr) return(true);
        return(false);
    }
    
    int getSize(){
        int ret = 0;
        for(int i = 0; i < size; i++){
            HashMapEntry<K, V> *p = &tbl[i];
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
            HashMapEntry<K, V> *p = &tbl[i];
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
            HashMapEntry<K, V> *p = &tbl[i];
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
            HashMapEntry<K, V> *p = &tbl[i];
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
    
    sfwdr::ssize_t getKeys(Array<K> &ret){
        ret.clear();
        for(int i = 0; i < size; i++){
            HashMapEntry<K, V> *p = &tbl[i];
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
        HashMapEntry<K, V> *target = _getEntry(key);
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
                HashMapEntry<K, V> *se = target->getNext();
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
    
    int hash(K key){
        return(key.getHash() % this->size);
    }
    
};

#endif
