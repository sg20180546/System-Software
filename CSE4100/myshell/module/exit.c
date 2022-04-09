#include "exit.h"


void exit_(char** argv){
    int i;
    for(i=jobs_front;i<jobs_rear;i++){
        pid_t p=jobs_list[i]->pgid;
        SEND_USR2(p);
    }
    while(waitpid(-1,NULL,0)>0);
    exit(0);
}