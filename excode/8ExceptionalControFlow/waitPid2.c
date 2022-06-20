#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
int main(){
    if(fork()==0){
        printf("9");
        fflush(stdout);
    }else{
        printf("0");
        waitpid(-1,NULL,0);
    }
    printf("3");fflush(stdout);
    printf("6");exit(0);
}

/*
        |   - 9 - 3 - 6 exit
main - fork - 0         waitpid - 3 - 6 exit
9360/36
9036/36
0936/36
*/