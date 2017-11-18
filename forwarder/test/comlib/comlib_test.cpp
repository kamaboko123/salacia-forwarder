#include <cstdio>
#include <cstring>
#include <cinttypes>
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
    
    printf("\n\n");
    
    unsigned int iValue = 0xAABBCCDD;
    unsigned char *pc = (unsigned char *)&iValue;
    
    printf("%X : %X : %X : %X\n" , pc[0] , pc[1] , pc[2] , pc[3]);
    iValue = comlib::ntohl(iValue);
    printf("%X : %X : %X : %X\n" , pc[0] , pc[1] , pc[2] , pc[3]);
    iValue = comlib::htonl(iValue);
    printf("%X : %X : %X : %X\n" , pc[0] , pc[1] , pc[2] , pc[3]);
    
    printf("%d\n", comlib::ndigit(1000));
    //printf("%s\n", comlib::uitoa(1000, NULL, 1));
    
    char chars[6] = {0};
    comlib::uitoa(1001, chars, sizeof(chars));
    printf("%s\n", chars);
    
    printf("\n");
    printf("[strncat]\n");
    char chars2[100] = "abc";
    chars2[4] = 'a';
    chars2[5] = 'a';
    chars2[6] = 'a';
    chars2[7] = 'a';
    chars2[8] = 'a';
    printf("%s\n", chars2);
    comlib::strncat(chars2, (char *)"123", 2);
    printf("%s\n", chars2);
    
    printf("[strcat]\n");
    char chars3[100] = "123";
    printf("%s\n", comlib::strcat(chars3, (char *)"aaa"));
    
    printf("strlen : \"abc\" -> %d\n", comlib::strlen((char *)"abc"));
    
    printf("[strchr]\n");
    char str[] = "string";
    char c = 'r';
    printf("[strchr] %s, %c, %s\n", str, c, comlib::strchr(str, c));
    printf("[strchr_index] %s, %c, %d\n", str, c, comlib::strchr_index(str, c));
    
    /*
    iValue = comlib::rbyte32(iValue);
    printf("%X : %X : %X : %X\n" , pc[0] , pc[1] , pc[2] , pc[3]);
    iValue = comlib::rbyte16((uint16_t)iValue);
    printf("%X : %X : %X : %X\n" , pc[0] , pc[1] , pc[2] , pc[3]);
    iValue = comlib::rbyte16((uint16_t)iValue);
    printf("%X : %X : %X : %X\n" , pc[0] , pc[1] , pc[2] , pc[3]);
    */
    return(0);
}
