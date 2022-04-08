#include "sig.h"

void sigchild_handler_child(int sig){
    while(waitpid(-1,NULL,0)>0);
}


void sigchild_handler(int sig){
   // send SIGCONT to myself
   pid_t pid;
   while( (pid=waitpid(-1,NULL,0))>0){
       if(pid==child_pid){
           child_pid=0;
           SEND_CONTINUE(parent_pid);
       }else{
           int index=find_jobs_by_pid(pid);
           jobs_list[index]->state=TERMINATED;
       }
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
        insert_jobs(child_pid,buf,SUSPENDED);
        printf("\n[%d] Stopped %s\n",jobs_rear-1,buf);
        child_pid=0;
    } 
}








