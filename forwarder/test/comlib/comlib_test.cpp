#include <cstdio>
#include <cstring>
#include "../../src/comlib.hpp"

#define N 2
#define S1 "1"
#define S2 "abc"

int main(void){
    char s1[N] = S1;
    char s2[] = S2;
    
    char s_s1[N] = S1;
    char s_s2[] = S2;
    
    comlib::strncpy(s1, s2, N);
    strncpy(s_s1, s_s2, N);
    
    for(int i = 0; i < N; i++){
        printf("[%02d]%02u(%c), %02u(%c)\n", i, s1[i], s1[i], s_s1[i], s_s1[i]);
    }
    
    return(0);
}
