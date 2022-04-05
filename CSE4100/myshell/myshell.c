#include "myshell.h"

// [1] 20878 background [1]+  Done
int main(){

    parent_pid=getpid();
    signal(SIGCHLD,sigchild_handler_parent);
    signal(SIGINT,sigint_handler);
    signal(SIGSTOP,sigtstp_handler);
    
    while(1){
        interpreter(cmdline);
    }

    return 0;
}

