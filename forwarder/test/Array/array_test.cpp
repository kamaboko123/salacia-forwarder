#include <cstdio>
#include "../../src/Array.hpp"

int main(void){
    Array<int *> *array = new Array<int *>();
    
    int *pi1 = new int(1);
    int *pi2 = new int(2);
    int *pi3 = new int(3);
    
    printf("size : %u\n", array->getSize());
    array->add(pi1);
    printf("size : %u\n", array->getSize());
    array->add(pi2);
    printf("size : %u\n", array->getSize());
    array->add(pi3);
    printf("size : %u\n", array->getSize());
    
    printf("get(1) : %d\n", *array->get(0));
    printf("get(2) : %d\n", *array->get(1));
    printf("get(3) : %d\n", *array->get(2));
    
    return(0);
}
