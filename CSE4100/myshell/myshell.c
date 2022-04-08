#include "myshell.h"

// [1] 20878 background [1]+  Done
int main(){
    
    parent_pid=getpid();
    
    signal(SIGCHLD,sigchild_handler);
    signal(SIGINT,sigint_handler);
    signal(SIGTSTP,sigtstp_handler);
    
    // signal(SIGTTOU,sigttou_handler);
    // signal(SIGTTIN,sigttin_handler);
    // signal(SIGPIPE,sigpipe_handler);


    while(1){
        interpreter(cmdline);
    }

    return 0;
}

