#include "sig.h"

void sigchild_handler_child(int sig){
    pid_t pid=getpid();
    while(waitpid(-1,NULL,0)>0);
    SEND_CONTINUE(pid);
}


void sigchild_handler(int sig){
   pid_t pid;
   while( (pid=waitpid(-1,NULL,WNOHANG))>0){
       if(pid==child_pid){
           child_pid=0;
        //    pd("here?!");
           SEND_CONTINUE(parent_pid);
       }else{
           
           int index=find_jobs_by_pid(pid);
           jobs_list[index]->state=TERMINATED;
       }
   }
    tcsetpgrp(STDIN_FILENO,getpid());
    tcsetpgrp(STDOUT_FILENO,getpid());
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
        printf("at handler%d\n",child_pid);
        SEND_TSTP(child_pid);
        
        insert_jobs(child_pid,buf,SUSPENDED);
        printf("[%d] Stopped %s\n",jobs_rear-1,buf);
        child_pid=0;
    }
    SEND_CONTINUE(parent_pid);
}

void sigtstp_handler_child(int sig){
    pid_t pid=getpid();
    SEND_CONTINUE(parent_pid);
    SEND_STOP(pid);
}


void sigttou_handler(int sig){
   tcsetpgrp(STDOUT_FILENO,getpid());
}

void sigttin_handler(int sig){
    tcsetpgrp(STDIN_FILENO,getpid());
}



