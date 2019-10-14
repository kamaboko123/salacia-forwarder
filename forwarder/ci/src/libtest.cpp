#include "libtest.hpp"

KeyMock::KeyMock(){
    key = 0;
}

KeyMock::KeyMock(int v){
    key = v;
}

int KeyMock::getHash(){
    return key;
}

int KeyMock::getKey(){
    return key;
}

bool KeyMock::operator==(KeyMock v){
    return(key == v.getKey());
}

