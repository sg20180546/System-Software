#include "jobs.h"

int jobs_rear=1; int jobs_n=0; int jobs_front=1;

static int job_parser(char* arg);
static int find_job_by_command(char* arg);

void insert_jobs(pid_t pgid,char* cmdline,STATE state){
    jobs_n++;
    JOB* job=malloc(sizeof(JOB));
    job->cmdline=malloc(strlen(cmdline));
    strcpy(job->cmdline,cmdline);
    job->pgid=pgid;
    job->state=state;
    // hashing ? array?
    jobs_list[jobs_rear++]=job;
}

void print_jobs(char** argv){
    int i;
    for(i=jobs_front;i<jobs_rear;i++){
        printf("[%d] ",i);
        switch(jobs_list[i]->state){
            case RUNNING:
                printf("RUNNING    ");
                break;
            case SUSPENDED:
                printf("SUSPENDED ");
                break;
            case TERMINATED:
                printf("TERMINATED ");
                break;
            default:
                break;
        }
        printf("%s PID %d\n",jobs_list[i]->cmdline,jobs_list[i]->pgid);
    }
}

void print_done_jobs(){
    int i=0;
    while(jobs_done[i]){
        printf("[%d] Done %s",jobs_done[i],jobs_list[jobs_done[i]]->cmdline);
        free(jobs_list[jobs_done[i]]->cmdline);
        free(jobs_list[jobs_done[i]]);
        jobs_done[i]=0;
        i++;
        jobs_n--;
    }
}

void reap_dead_jobs(){
    int i=jobs_front;
    int j=0;
    for(;i<jobs_rear;i++){
        if(jobs_list[i]->state==TERMINATED){
            jobs_done[j++]=i;
        }
    }
    if(jobs_n==0){
        jobs_front=1;
        jobs_rear=1;
    }
}

int find_jobs_by_pid(pid_t pid){
    int ret=-1;
    int i=jobs_front;
    while(!jobs_list[i]) i++;
    jobs_front=i;
    for(;i<jobs_rear;i++){
        if(jobs_list[i]->pgid==pid) ret=i;
    }
    return ret;
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
    if(argv[1][0]!='%'){
        fprintf(stderr,"Usage : %s %%<job spec>",argv[0]);
    }
    int index=job_parser(&argv[1][1]);
    pid_t child_p;
    if(index){
        child_p=jobs_list[index]->pgid;
        if(jobs_list[index]->state!=TERMINATED){

        }else{

        }
    }else{
        child_p=find_job_by_command(argv[1]);
    }
    child_pid=child_p;
    SEND_STOP(parent_pid);
    SEND_CONTINUE(child_p);
    // parse argv[1]
    // if(argv[1]==number) find process in list
    // if(argv[1]==name) find process in list
    // IF not exist, return with notfounderror

    // send signal to parent to sleep

    // send signal to child to SIGCONT

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