#include "sig.h"

void sigchild_handler_child(int sig){
    pid_t pid=getpid();
    int st;
    while(waitpid(0,&st,WUNTRACED)>0){
        if(WIFSTOPPED(st)){
            // SEND_CONTINUE(parent_pid);
            SEND_STOP(pid);
        }
    }
    SEND_INT(parent_pid);
}


void sigchild_handler(int sig){
    int st;
   pid_t pid;
   while( (pid=waitpid(-1,&st,WNOHANG|WUNTRACED))>0){
       if(WIFSTOPPED(st)){
        insert_jobs(child_pid,buf,SUSPENDED);
        printf("[%d] Stopped %s\n",jobs_rear-1,buf);
        SEND_CONTINUE(parent_pid);
       }else if(WIFEXITED(st)){
            if(pid==child_pid){
                child_pid=0;
                SEND_CONTINUE(parent_pid);
            }else{
                int index=find_jobs_by_pid(pid);
                if(index!=-1) jobs_list[index]->state=TERMINATED;
            }
       }else{
        //    sigsuspend(&mask);
       }
   }
    tcsetpgrp(STDIN_FILENO,parent_pid);
    tcsetpgrp(STDOUT_FILENO,parent_pid);
}



void sigint_handler(int sig){
    
    if(child_pid) {
        SEND_INT(child_pid);
        waitpid(child_pid,NULL,0);


        child_pid=0;
    }
    SEND_CONTINUE(parent_pid);
}



void sigtstp_handler(int sig){
    tcsetpgrp(STDIN_FILENO,parent_pid);
    tcsetpgrp(STDOUT_FILENO,parent_pid);
    child_pid=0;
    SEND_CONTINUE(parent_pid);
}


void sigusr1_handler(int sig){
    int i;
    pid_t p;
    for(i=0;pid_list[i];i++){  
        p=pid_list[i];
        SEND_CONTINUE(p);
    }
    p=getpid();
    tcsetpgrp(STDIN_FILENO,p);
    tcsetpgrp(STDOUT_FILENO,p);
    sigsuspend(&mask);
}
void sigusr2_handler(int sig){
    int i;

    pid_t p;
    for(i=0;pid_list[i];i++){
        p=pid_list[i];
        SEND_KILL(p);
    }
    _exit(0);
}




