#include "myshell.h"
static void initialize();

int main(){
    #ifdef DEBUG
        pd("DEBUGGING MODE");
    #endif
    initialize();

    while(1){
        interpreter(cmdline);
    }

    return 0;
}


static void initialize(){
    sigemptyset(&mask);
    parent_pid=getpid();
    signal(SIGCHLD,sigchild_handler);
    signal(SIGINT,sigint_handler);
    signal(SIGTSTP,sigtstp_handler);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTTOU,SIG_IGN);
    signal(SIGUSR1,SIG_IGN);
    signal(SIGUSR2,SIG_IGN);
    jobs_front=1;
    jobs_rear=1;
    jobs_n=0;
}