#include "jobs.h"

static int job_index_parser(char* arg);
static int find_job_by_command(char* arg);

void insert_jobs(pid_t pgid,char* cmdline,STATE state){
    jobs_n++;
    JOB* job=malloc(sizeof(JOB));
    job->cmdline=malloc(strlen(cmdline));
    strcpy(job->cmdline,cmdline);
    job->pgid=pgid;
    job->state=state;
    jobs_list[jobs_rear++]=job;
}

void print_jobs(char** argv){
    int i;
    for(i=jobs_front;i<jobs_rear;i++){
        if(!jobs_list[i]->cmdline) continue;
        fprintf(stdout,"[%d] ",i);
        switch(jobs_list[i]->state){
            case RUNNING:
                printf("RUNNING   ");
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
        if(jobs_list[i]->cmdline) printf("%s",jobs_list[i]->cmdline);
    }
}

void print_done_jobs(){
    int i=0;
    int flag=0;
    while(jobs_done[i]){
        printf("[%d] Done %s",jobs_done[i],jobs_list[jobs_done[i]]->cmdline);
        safe_free(jobs_list[jobs_done[i]]->cmdline);
        safe_free(jobs_list[jobs_done[i]]);
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
        if(jobs_list[i]->pgid==pid){
            ret=i;
            break;
        }
    }
    return ret;
}



// bg argv[1] -> PROCESS LIST NUMBER , PROCESS NAME
// hashing ? 
// fg bug -> sleep 30 / cltz / fg %1 / clt z
void fg(char** argv){
    int st;
    fflush(stdin); fflush(stdout);
    if(!argv[1]||!argv[1][1] ){
        fprintf(stderr,"Usage : %s %%<job spec>\n",argv[0]);
        return;
    }
    int index=job_index_parser(&argv[1][1]);

    
    if(!index){
        index=find_job_by_command(argv[1]);
    }else if(argv[1][0]!='%'){
        fprintf(stderr,"Usage : %s %%<job spec>",argv[0]);
    }
    if(!jobs_list[index]){ 
        fprintf(stderr,"No Such job\n");
        return;
    }
    if(jobs_list[index]->state==TERMINATED){
        fprintf(stderr,"[%d] Already Terminated job\n",index);
        return;
    }
    
    child_pid=jobs_list[index]->pgid;
    printf("Resumed [%d] %s",index,jobs_list[index]->cmdline);
    strcpy(buf,jobs_list[index]->cmdline);
    free(jobs_list[index]->cmdline); 
    free(jobs_list[index]);
    jobs_n--;
    tcsetpgrp(child_pid,STDIN_FILENO);
    tcsetpgrp(child_pid,STDOUT_FILENO);
    SEND_CONTINUE(child_pid);
    SEND_USR1(child_pid);
    
    waitpid(child_pid,&st,WUNTRACED);
    if(WIFSTOPPED(st)){
        insert_jobs(child_pid,buf,SUSPENDED);
        printf("[%d] Stopped %s",jobs_rear-1,buf);
    }
}



void bg(char** argv){
    if(!argv[1]||!argv[1][1] ){
        fprintf(stderr,"Usage : %s %%<job spec>\n",argv[0]);
        return;
    }
    int index=job_index_parser(&argv[1][1]);
    pid_t child_p;

    if(!index){
        index=find_job_by_command(argv[1]);
    }else if(argv[1][0]!='%'){
        fprintf(stderr,"Usage : %s %%<job spec>",argv[0]);
    }
    if(!jobs_list[index]){
        fprintf(stderr,"No Such job\n");
        return;
    } 
    if(jobs_list[index]->state==TERMINATED){
        fprintf(stderr,"[%d] Already Terminated job\n",index);
        return;
    }
    jobs_list[index]->state=RUNNING;
    child_p=jobs_list[index]->pgid;
    printf("[%d] %s",index,jobs_list[index]->cmdline);
    SEND_CONTINUE(child_p);
    SEND_USR1(child_p);
}

void kill_job(char** argv){
    if(!argv[1]||!argv[1][1] ){
        fprintf(stderr,"Usage : %s %%<job spec>\n",argv[0]);
        return;
    }
    int index=job_index_parser(&argv[1][1]);
    pid_t child_p;

    if(!index){
        index=find_job_by_command(argv[1]);
    }else if(argv[1][0]!='%'){
        fprintf(stderr,"Usage : %s %%<job spec>",argv[0]);
    }
    if(!jobs_list[index]){
        fprintf(stderr,"No Such job\n");
        return;
    } 
    if(jobs_list[index]->state==TERMINATED){
        fprintf(stderr,"[%d] Already Terminated job\n",index);
        return;
    }
    jobs_list[index]->state=TERMINATED;
    child_p=jobs_list[index]->pgid;
    SEND_USR2(child_p);

}


static int job_index_parser(char* arg){
    
    int index=atoi(arg);
    char *buf=malloc(strlen(arg));
    sprintf(buf,"%d",index);
    if(strcmp(buf,arg)!=0){
        index=-1;
    }
    safe_free(buf);
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