#include <stdio.h>
#include <net/if.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if(argc == 2){
        int ifindex;
        char ifname[IF_NAMESIZE];
        
        ifindex = atoi(argv[1]);
        
        if(ifindex == 0){
            fprintf(stdout, "Invaild Args. ifindex is must integer.\n");
            return(-1);
        }
        
        if(if_indextoname(ifindex, ifname) != NULL){
            //printf("ifindex = %d , ifname = %s\n", ifindex, ifname);
            printf("%s\n", ifname);
            return(0);
        }
        else{
            fprintf(stdout, "ifindex %d is not found.\n", ifindex);
            return(1);
        }
    }
    fprintf(stdout, "Invaild Args\n");
    return(-1);
}
