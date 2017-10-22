#include <iostream>

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
    
    /*
    bool del(){
        if(getNext() == nullptr){
            if(getPrev() == nullptr){a
                //最初(次なし)
                return false;
            }
            //最後
            getPrev()->setNext(nullptr);
            init();
            return(true);
        }

    /*
    HashMapEntry(K key){
        
    }*/
    
};

template <typename K, typename V>
class HashMap{
private:
    HashMapEntry<K, V> *tbl;
    int size;
    
    HashMapEntry<K, V> *getEntry(K key){
        int index = hash(key);
        if(tbl[index].isEmpty()) return(nullptr);
        
        if(tbl[index].getKey() == key){
            return(&tbl[index]);
        }
        
        HashMapEntry<K, V> *p = &tbl[index];
        do{
            p = p->getNext();
            if(p->getKey() == key){
                return(p);
            }
        }while(p->getNext() != nullptr);
        
        return(nullptr);
    }
    
public:
    HashMap<K, V>(int size){
        this->size = size;
        this->tbl = new HashMapEntry<K, V>[this->size];
    }
    
    ~HashMap(){
        std::cout << "destructer HashMap" << std::endl;
        
        //TBD : 全部deleteする処理書く
        //delete this->tbl;
        int size = getSize();
        K *keys = new K[size];
        getKeys(keys);
        
        for(int i = 0; i < size; i++){
            std::cout << "delete : ";
            std::cout << keys[i].toInt() << std::endl;
            /*
            if(getEntry(keys[i])->getPrev() != nullptr){
                del(keys[i]);
            }*/
            del(keys[i]);
        }
        
        delete[] keys;
        delete[] this->tbl;
    }
    
    /*
    void update(){
        
    }*/
    
    void update(K key, V value){
        
        //std::cout << getSize() << std::endl;
        int index = hash(key);
        if(tbl[index].isEmpty()){
            //fist node is empty
            tbl[index].set(key, value, nullptr, nullptr);
            //std::cout << "d" << std::endl;
            return;
        }
        
        HashMapEntry<K, V> *p;
        p = &(tbl[index]);
        while(p->getNext() != nullptr){
            //keyが同じなら中身更新
            if(p->getKey() == key){
                std::cout << "d" << std::endl;
                p->setValue(value);
                return;
            }
            p = p->getNext();
        }
        
        //last node
        //keyが同じなら中身更新
        if(p->getKey() == key){
            std::cout << "d" << std::endl;
            p->setValue(value);
            return;
        }
        
        //create new node
        HashMapEntry<K, V> *ne = new HashMapEntry<K, V>();
        p->setNext(ne);
        //p->getNext()->set(key, value, nullptr, p);
        ne->set(key, value, nullptr, p);
    }
    
    V get(K key){
        return(getEntry(key)->get());
        /*
        int index = hash(key);
        if(tbl[index].isEmpty()) return(nullptr);
        
        if(tbl[index].getKey() == key){
            return(tbl[index].get());
        }
        
        HashMapEntry<K, V> *p = &tbl[index];
        do{
            p = p->getNext();
            if(p->getKey() == key){
                return(p->get());
            }
        }while(p->getNext() != nullptr);
        
        return(nullptr);
        */
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
    
    K *getKeys(){
        K *ret = new K[getSize()];
        int j = 0;
        for(int i = 0; i < size; i++){
            HashMapEntry<K, V> *p = &tbl[i];
            if(p->isEmpty()) continue;
            ret[j] = tbl[i].getKey();
            //std::cout << tbl[i].getKey().toInt() << std::endl;
            j++;
            while(p->getNext() != nullptr){
                //std::cout << p->getNext()->getKey().toInt() << std::endl;
                ret[j] = p->getNext()->getKey();
                j++;
                p = p->getNext();
            }
        }
        return(ret);
    }
    
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
    
    bool del(K key){
        HashMapEntry<K, V> *target = getEntry(key);
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

