#include<stdio.h>
#include<string.h>

int main(void)
{
    char c[2]="";
    //memset(&c, 0, sizeof(c));
    printf("%.2x %.2x\n", c[0], c[1]);
    
    (int)*c = 48;
    printf("%.2x %.2x\n", c[0], c[1]);
    return(0);
}
