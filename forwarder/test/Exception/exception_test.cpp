#include <cstdio>
#include "Exception.hpp"

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
    
    try{
        throw Exception::InvalidEthType((char *)"ARP");
    }
    catch(Exception::InvalidEthType &e){
        printf("%s\n", e.getMessage());
    }
    
    return(0);
}
