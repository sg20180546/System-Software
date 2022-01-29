#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
    int x=1;
    pid_t ppid=getppid();
    pid_t forkpid=fork();
    pid_t curpid=getpid();
    if(forkpid==0){
        printf("child : pid-%d,ppid-%d ,fork-%d\n",curpid,ppid,forkpid);
        exit(0);
    }else{
        printf("parent pid-%d,ppid-%d ,fork-%d\n",curpid,ppid,forkpid);
        exit(0);
    }
}