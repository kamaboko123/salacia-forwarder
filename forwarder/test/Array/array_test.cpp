#include <cstdio>
#include "../../src/Array.hpp"

void dump(Array<int *> *array){
    printf("\n---[dump]---\n");
    printf("size : %d\n", array->getSize());
    for(uint32_t i = 0; i < array->getSize(); i++){
        printf("index[%d] : %d\n", i, *array->get(i));
    }
    printf("---[dump]---\n\n");
}

void add(Array<int *> *array, int *item){
    printf("add : %d\n", *item);
    array->add(item);
}

void set(Array<int *> *array, int index, int *item){
    printf("set : index[%d] = %d\n", index, *item);
    array->set(index, item);
}

void del(Array<int *> *array, int index){
    if(array->del(index) == false){
        printf("del:index[%d](failed)\n", index);
        return;
    }
    printf("del:index[%d]\n", index);
}

void copy_test(Array<int *> array){
    printf("copy by function argment\n");
}

int main(void){
    Array<int *> *array = new Array<int *>();
    
    int *pi0 = new int(0);
    int *pi1 = new int(1);
    int *pi2 = new int(2);
    int *pi3 = new int(3);
    //int *pi4 = new int(4);
    //int *pi5 = new int(5);
    
    dump(array);
    
    add(array, pi0);
    add(array, pi1);
    add(array, pi2);
    add(array, pi3);
    //add(array, pi4);
    //add(array, pi5);
    
    dump(array);
    
    del(array, 4);
    del(array, 1);
    
    dump(array);
    set(array, 1, pi1);
    
    dump(array);
    
    printf("copy\n");
    Array<int *> array_copied;
    array_copied = *array;
    dump(&array_copied);
    
    copy_test(*array);
    
    try{
        array->get(3);
    }
    catch(sfwdr::Exception::OutOfRange ex){
        printf("%s\n", ex.getMessage());
    }
    
    delete pi0;
    delete pi1;
    delete pi2;
    delete pi3;
    //delete pi4;
    //delete pi5;
    
    delete array;
    return(0);
}
