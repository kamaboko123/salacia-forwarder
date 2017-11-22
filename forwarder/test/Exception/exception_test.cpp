#include <cstdio>
#include "../../src/Exception.hpp"

using namespace sfwdr;

int main(void){
    try{
        throw Exception::OutOfRange();
        //throw Exception::Exception((char *)"exception");
    }
    catch(Exception::OutOfRange &e){
        printf("[OutOfRange]%s\n", e.getMessage());
    }
    catch(Exception::Exception &e){
        printf("[Exception]%s\n", e.getMessage());
    }
    
    return(0);
}
