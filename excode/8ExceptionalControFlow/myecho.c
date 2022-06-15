#include <stdlib.h>
#include <stdio.h>

int main(int argc,char* argv[],char*envp[] ){
    int i=0;
    printf("Command-line arguments:\n");
    while(argv[i]!=NULL){
        printf("argv[%d]: %s\n",i,argv[i]);
        i++;
    }
    i=0;
    while(envp[i]!=NULL){
        printf("envp[%d]: %s\n",i,envp[i]);
        i++;
    }
    printf("argc : %d\n",argc);
    return 0;
}