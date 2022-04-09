#include "exit.h"


void exit_(char** argv){
    int i;
    if(child_pid) SEND_KILL(child_pid);
    for(i=jobs_front;i<jobs_rear;i++){
        pid_t p=jobs_list[i]->pgid;
        SEND_KILL(p);
    }
    while(waitpid(-1,NULL,0)>0);
    exit(0);
}