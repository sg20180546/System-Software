#include "sig.h"

void sigchild_handler_child(int sig){
    while(waitpid(-1,NULL,0)>0){
        kill(getpid(),SIGCONT);
    }
}

void sigchild_handler(int sig){
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
    
    // pd("sig int");
    if(child_pid) {
        // printf("%d",child_pid);
        SEND_INT(child_pid);
        SEND_CONTINUE(parent_pid);
        child_pid=0;
    }
}

// void sigint_handler_child(int sig){
//     SEND_CONTINUE(parent_pid);
//     kill(getpid(),SIGKILL);
// }

void sigtstp_handler(int sig){
    
    // pd("sig STOP");
    if(child_pid){

        printf("%d",child_pid);
        SEND_TSTP(child_pid);
        SEND_CONTINUE(parent_pid);
        pd("Stopped");
        child_pid=0;
    } 
}

// void sigtstp_handler_child(int sig){
//     // SEND_CONTINUE(parent_pid);
//     kill(getpid(),SIGSTOP);
// }










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