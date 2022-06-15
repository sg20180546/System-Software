#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
int main(){
    pid_t pid;
    if((pid=fork())==0){
        // at child, fork return 0
        pause(); //child pause
        printf("control should never reach here\n");
    }
// at parent, fork return child process
    kill(pid,SIGKILL); //kill child
    exit(0); //parent exit
}