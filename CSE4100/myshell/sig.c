#include "sig.h"

void sigchild_handler_child(int sig){
    while(waitpid(-1,NULL,0)>0){

    }
    SEND_CONTINUE(parent_pid);
}

void sigchild_handler(int sig){
   // send SIGCONT to myself
   pid_t pid;
   while( (pid=waitpid(-1,NULL,0))>0){
       if(pid==child_pid){
           child_pid=0;
           SEND_CONTINUE(parent_pid);
       }
       // access to jobs, make state running to dead
   }
}


void sigint_handler(int sig){
    
    if(child_pid) {
        SEND_INT(child_pid);
        waitpid(child_pid,NULL,0);
        SEND_CONTINUE(parent_pid);
        child_pid=0;
    }
}



void sigtstp_handler(int sig){
    if(child_pid){

        printf("%d",child_pid);
        SEND_TSTP(child_pid);
        SEND_CONTINUE(parent_pid);
        pd("Stopped");
        child_pid=0;
    } 
}










void sigttou_handler(int sig){
    SEND_KILL(parent_pid);
    exit(0);    
    fprintf(stderr,"sig ttou come!\n");
    kill(parent_pid,SIGCONT);
}


void sigttin_handler(int sig){
    fprintf(stderr,"sig ttin come!\n");
}

void sigpipe_handler(int sig){
    fprintf(stderr,"sig pipe come!\n");
}