#ifndef INCLUDED_ARRAY
#define INCLUDED_ARRAY

#include "Exception.hpp"

template <typename T>
class ArrayItem{
private:
    T item;
    ArrayItem<T> *next;
    ArrayItem<T> *prev;

public:
    ArrayItem<T>(T item){
        this->item = item;
        next = nullptr;
    }
    
    T get(){
        return(item);
    }
    
    void set(T item){
        this->item = item;
    }
    
    void setNext(ArrayItem<T> *next_item){
        next = next_item;
    }
    
    ArrayItem<T> *getNext(){
        return(next);
    }
    
};

template <typename T>
class Array{
private:
    uint32_t size;
    ArrayItem<T> *root;
    
    ArrayItem<T> *getArrayItem(uint32_t index){
        if(index >= size) return(nullptr);
        
        ArrayItem<T> *p = root;
        for(uint32_t i = 0; i < index; i++){
            p = p->getNext();
        }
        return(p);
    }
    
public:
    Array<T>(){
        size = 0;
        root = nullptr;
    }
    
    ~Array<T>(){
        clear();
    }
    
    void clear(){
        uint32_t s = size;
        for(uint32_t i = 0; i < s; i++){
            del(0);
        }
    }
    
    void add(T item){
        if(root == nullptr){
            root = new ArrayItem<T>(item);
            size++;
            return;
        }
        
        ArrayItem<T> *p = root;
        for(uint32_t i = 0; i < size - 1; i++){
            p = p->getNext();
        }
        
        p->setNext(new ArrayItem<T>(item));
        size++;
    }
    
    bool set(uint32_t index, T item){
        ArrayItem<T> *target = getArrayItem(index);
        if(target == nullptr){
            return(false);
        }
        target->set(item);
        return(true);
    }
    
    bool del(uint32_t index){
        if(index >= size) return(false);
        
        //root node
        if(index == 0){
            ArrayItem<T> *n_root = root->getNext();
            delete root;
            root = n_root;
            size--;
            
            return(true);
        }
        
        ArrayItem<T> *prev = getArrayItem(index-1);
        ArrayItem<T> *target = getArrayItem(index);
        
        prev->setNext(target->getNext());
        delete target;
        size--;
        return(true);
    }
    
    uint32_t getSize(){
        return(size);
    }
    
    T get(uint32_t index){
        ArrayItem<T> *ret = getArrayItem(index);
        if(ret == nullptr){
            throw sfwdr::Exception::OutOfRange();
            //return(nullptr);
        }
        
        return(ret->get());
    }
};

#endif
