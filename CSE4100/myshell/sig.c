#include "sig.h"

void sigchild_handler_child(int sig){
    pid_t pid=getpid();
    while(waitpid(0,NULL,0)>0);

    SEND_CONTINUE(pid);
}


void sigchild_handler(int sig){
   pid_t pid;
   while( (pid=waitpid(-1,NULL,WNOHANG))>0){
       if(pid==child_pid){
           child_pid=0;
           SEND_CONTINUE(parent_pid);
       }else{
           
           int index=find_jobs_by_pid(pid);
           jobs_list[index]->state=TERMINATED;
       }
   }
    tcsetpgrp(STDIN_FILENO,parent_pid);
    tcsetpgrp(STDOUT_FILENO,parent_pid);
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
        // printf("at handler%d\n",child_pid);
        
        
        insert_jobs(child_pid,buf,SUSPENDED);
        printf("[%d] Stopped %s\n",jobs_rear-1,buf);
    }
    child_pid=0;
    SEND_CONTINUE(parent_pid);
}

void sigtstp_handler_child(int sig){
    pid_t pid=getpid();
    SEND_TSTP(parent_pid);
    tcsetpgrp(STDIN_FILENO,parent_pid);
    tcsetpgrp(STDOUT_FILENO,parent_pid);
    SEND_STOP(pid);
}

void sigusr1_handler(int sig){
    int i;
    pid_t p;
    for(i=0;pid_list[i];i++){  
        p=pid_list[i];
        SEND_CONTINUE(p);
    }
    tcsetpgrp(STDIN_FILENO,getpid());
    tcsetpgrp(STDOUT_FILENO,getpid());
    sigsuspend(&mask);
}
void sigusr2_handler(int sig){
    int i;

    pid_t p;
    for(i=0;pid_list[i];i++){
        p=pid_list[i];
        // fprintf(stderr,"in fork : %d\n",p);
        SEND_KILL(p);
    }
    // p=getpid();
    // SEND_KILL(p);
    _exit(0);
}




