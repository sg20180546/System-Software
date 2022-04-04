#include "jobs.h"

p_front=0; p_rear=0;


// bg argv[1] -> PROCESS LIST NUMBER , PROCESS NAME
// hashing ? 
void bg(char** argv){
    // parse argv[1]
    // if(argv[1]==number) find process in list
    // if(argv[1]==name) find process in list
    fflush(STDOUT_FILENO); fflush(STDIN_FILENO);
}
void fg(char** argv){
    pid_t child;
    // parse argv[1]
    // if(argv[1]==number) find process in list
    // if(argv[1]==name) find process in list
    // IF not exist, return with notfounderror

    // send signal to parent to sleep
    kill(parent_pid,SIGSTOP);
    // send signal to child to SIGCONT
    kill(child,SIGCONT);
    // after child, parent getup
}