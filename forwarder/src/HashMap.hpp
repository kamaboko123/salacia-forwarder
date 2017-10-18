#include <iostream>

template <typename T>
class HashMapEntry{
private:
    T content;
    HashMapEntry<T> *next;
    HashMapEntry<T> *prev;
    
public:
    HashMapEntry<T>(){
        init();
    }
    
    void init(){
        std::cout << "init" << std::endl;
        this->next = nullptr;
        this->prev = nullptr;
    }
    
};

template <typename K, typename V>
class HashMap{
private:
    //1,2
    HashMapEntry<V> *tbl;
    
    //3
    //HashMapEntry<V> **tbl;
    
public:
    HashMap<K, V>(int size){
        //1. こっちにするか
        this->tbl = new HashMapEntry<V>[size];
        
        /*
        //2. こっちにするか
        tbl = (HashMapEntry<V> *)malloc(sizeof(HashMapEntry<V>)*size);
        for(int i = 0; i < size; i++){
            tbl[i].init();
        }
        
        //3.こんなのもある
        this->tbl = new HashMapEntry<V>*[size];
        for(int i = 0; i < size; i++){
            tbl[i] = new HashMapEntry<V>();
        }
        */
        
    }
};

