#include <stdio.h>
#include <stdint.h>

unsigned int iptoi(char *addr);

unsigned int iptoi(char *addr){
    //char addr[] = "1.1.1.1";
    unsigned char octet[4][4];
    int i= 0, j = 0, k = 0;
    char buf[4];
    unsigned int dec = 0;
    
    for(i=0; i < 4; i++){
        for(j=0; j < 4; j++){
            octet[i][j] = 0;
            //printf("%d\n", octet[i][j]);
        }
    }
    
    j = 0;
    k = 0;
    for (i=0; addr[i] != '\0'; i++){
        if (addr[i] == '.'){
            octet[j][k] = '\0';
            j++;
            k = 0;
            continue;
        }
        octet[j][k] = addr[i];
        k++;
    }
    
    for(i=0; i < 4; i++){
        dec += atoi(octet[i]) << 8*(3-i);
        printf("%s %u\n", octet[i], dec);
        /*
        if(i < 3){
            dec = dec << 8;
        }*/
    }
    
    printf("%s\n", addr);
    printf("!%u\n", dec);
    return(dec);
}
