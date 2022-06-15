#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
int main(){
    int x=1;
    pid_t ppid=getppid();
    pid_t forkpid=fork();
    pid_t curpid=getpid();
    if(forkpid==0){
        printf("child : pid-%d,ppid-%d ,fork-%d\n",curpid,ppid,forkpid);
        exit(0);
    }else{
        pid_t wp;
        //WUNTRACED : wait before child process terminated
        waitpid(-1,NULL,WUNTRACED);
        //WNOHANG: if there is no termiated process, return 0
        // waitpid(-1,NULL,WNOHANG);
        while(1){
            wp=waitpid(-1,NULL,WNOHANG);
            if(wp!=0){
                break;
            }
            pid_t pid=getpid();
            printf("wating...%d\n",pid);
        }
        printf("wp: %d\n",wp);
        printf("parent pid-%d,ppid-%d ,fork-%d\n",curpid,ppid,forkpid);
        exit(0);
    }
}
/*
WNOHANG
wating...40809
wating...40809
wating...40809
wating...40809
wating...40809
wating...40809
child : pid-40810,ppid-25242 ,fork-0
wating...40809
wating...40809
wating...40809
wp: 40810
parent pid-40809,ppid-25242 ,fork-40810

WUNTRACED
child : pid-41087,ppid-25242 ,fork-0
wp: -1
parent pid-41086,ppid-25242 ,fork-41087


*/