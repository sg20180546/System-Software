#include "jobs.h"

int jobs_rear=0; int jobs_n=0; int jobs_front=0;

static int job_parser(char* arg);
static int find_job_by_command(char* arg);

void insert_jobs(pid_t pgid,char* cmdline,int foreground){
    JOB* job=malloc(sizeof(JOB));
    job->cmdline=malloc(strlen(cmdline));
    strcpy(job->cmdline,cmdline);
    job->foreground=foreground;
    job->pgid=pgid;
    job->state=RUNNING;
    // hashing ? array?
    jobs_list[jobs_rear++]=job;
}

void print_jobs(char** argv){

}

void reap_dead_jobs(){

}



// bg argv[1] -> PROCESS LIST NUMBER , PROCESS NAME
// hashing ? 
void bg(char** argv){
    pid_t child=job_parser(argv[1]);

    if(child==-1){
        int index=find_job_by_command(argv[1]);
        if(index) child=jobs_list[index]->pgid;
    }
    if(!child){
        fprintf(stderr,"No such jobs\n");
    }
    // parse argv[1]
    // if(argv[1]==number) find process in list
    // if(argv[1]==name) find process in list

    // send signal to bg job to continue
    kill(child,SIGCONT);
    fflush(stdin); fflush(stdout);
}
void fg(char** argv){
    pid_t child;
    // parse argv[1]
    // if(argv[1]==number) find process in list
    // if(argv[1]==name) find process in list
    // IF not exist, return with notfounderror

    // send signal to parent to sleep
    kill(parent_pid,SIGSTOP);
    // send signal to child to SIGCONT
    kill(child,SIGCONT);
    // after child, parent getup
}


static int job_parser(char* arg){
    int index=atoi(arg);
    char *buf=malloc(strlen(arg));
    sprintf(buf,"%d",index);
    if(strcmp(buf,arg)!=0){
        index=-1;
    }
    free(buf);
    return index;
}
static int find_job_by_command(char* arg){
    int index=-1;
    for(int i=0;i<jobs_rear;i++){
        
        if(jobs_list[i]!=NULL&&!strcmp(arg,jobs_list[i]->cmdline)){
            index=i;
            break;
        }
    }
    return index;
}