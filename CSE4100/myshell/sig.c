#include "sig.h"


void sigchild_handler_child(int sig){
    while(waitpid(-1,NULL,0)>0);
}

void sigchild_handler_parent(int sig){
   // send SIGCONT to myself
   pid_t pid;
   while( (pid=waitpid(-1,NULL,0))>0){
       // access to jobs, make state running to dead
   }
   kill(parent_pid,SIGCONT);
}


void sigint_handler(int sig){

}

void sigtstp_handler(int sig){

}