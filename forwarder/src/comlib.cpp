#include "comlib.hpp"

/**
 * copy s2 to s1
 */
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

/**
 * convert byte array to long(uint64_t)
 */
uint64_t comlib::bytestol(uint8_t *head, int n){
    uint64_t ret = 0;
    for(int i = 0; i < n; i++){
        ret += ((uint64_t)head[i] << (8 * (n - i - 1)));
        printf("%d\n", (8 * (n - i - 1)));
    }
    return(ret);
}
