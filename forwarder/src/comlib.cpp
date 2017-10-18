#include "comlib.hpp"

void comlib::strncpy(char *s1, char *s2, int n){
    bool end = false;
    for(int i = 0; i < n; i++){
        if(end){
            s1[i] = '\0';
            continue;
        }
        if(s2[i] == '\0'){
            s1[i] = '\0';
            end = true;
        }
        s1[i] = s2[i];
    }
}
