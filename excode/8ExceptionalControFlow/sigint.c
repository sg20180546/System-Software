#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void sigint_handler(int sig){
    printf("Caught SIGINT!\n");
    exit(0);
}

int main(){
    if(signal(SIGINT,sigint_handler)==SIG_ERR){
        fprintf(stderr,"signal error");
    }
    pause();
    return 0;
}