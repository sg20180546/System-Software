#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

int main(){
    int status;
    pid_t pid;
    printf("Start\n");
    pid=fork();
    printf("%d\n",!pid);
    if(pid==0){
        printf("child\n");
    }
    else if((waitpid(-1,&status,0))>0 && WIFEXITED(status)!=0){
        printf("%d\n",WEXITSTATUS(status));
    }
    printf("Stop\n");
    exit(2);
}

/*
      fork - 1 - child - Stop - exit
start -      0                   | - 2 - Stop - exit
*/