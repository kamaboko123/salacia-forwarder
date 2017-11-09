#ifndef INCLUDED_ARRAY
#define INCLUDED_ARRAY

#include <cstdint>

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
    
public:
    Array<T>(){
        size = 0;
        root = nullptr;
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
    
    /*
    void del(uint32_t index){
        a
    }*/
    
    uint32_t getSize(){
        return(size);
    }
    
    T get(uint32_t index){
        if(index >= size) return(nullptr);
        
        ArrayItem<T> *p = root;
        for(uint32_t i = 0; i < index; i++){
            p = p->getNext();
        }
        return(p->get());
    }
};

#endif
