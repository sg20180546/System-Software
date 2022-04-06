#include "sig.h"

void sigchild_handler_child(int sig){
    while(waitpid(-1,NULL,0)>0);
}

void sigchild_handler_parent(int sig){
   // send SIGCONT to myself
   pid_t pid;
   while( (pid=waitpid(-1,NULL,0))>0){
       if(pid==child_pid){
           child_pid=0;
           kill(parent_pid,SIGCONT);
       }
       // access to jobs, make state running to dead
   }
}


void sigint_handler(int sig){
    if(child_pid) {
        child_pid=0;
        kill(child_pid,SIGINT);
    }
    else write(1,"\n",1);
    siglongjmp(jbuf,1);
}

void sigtstp_handler(int sig){
    if(child_pid) kill(child_pid,SIGSTOP);
    else write(1,"\n",1);
    siglongjmp(jbuf,1);
}