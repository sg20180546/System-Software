#include "common_test.h"


static pid_t fork_and_exec(char** argv){
    pid_t pid=fork();
    if(pid==0){
        if(execv(argv[0],argv)<0){
            write(1,"exec failed",12);
            exit(0);
        }
    }
    return pid;
}

inline static void print_execution_time(struct timeval s){
    struct timeval e;
    gettimeofday(&e,NULL);
    double time_taken=e.tv_sec+e.tv_usec/1e6
                        -start.tv_sec-start.tv_usec/1e6;
    printf("Execution Time ; %f sec.\n",time_taken);
}

static char* getserveripaddr(){
    struct ifaddrs** addrs,*tmp;
    char*ret;
    getifaddrs((struct ifaddrs**)&addrs);
    tmp=(struct ifaddrs* )addrs;
    while(tmp){
        if(tmp->ifa_addr&&tmp->ifa_addr->sa_family==AF_INET){
            struct sockaddr_in *pAddr=(struct sockaddr_in*)tmp->ifa_addr;
            ret=malloc(strlen(inet_ntoa(pAddr->sin_addr)));
            strcpy(ret,inet_ntoa(pAddr->sin_addr));
            freeifaddrs((struct ifaddrs*)addrs);
            return ret;
        }
        tmp=tmp->ifa_next;
    }
}

void client_reaper(int sig){
    pid_t p;
    while((p=waitpid(0,NULL,WNOHANG))>0){
        if(p==pid_client) kill(getpid(),SIGCONT);
    }
}

void endclock(int sig){
    kill(getpid(),SIGCONT);
}

int main(int argc,char** argv){

    sigset_t ss;
    char* port,*server_ipaddr,*client_n,*thread_n=NULL,*show_r=NULL,*buy_r=NULL,*sell_r=NULL,*total_order;
    char* SERVER_ELF_PATH;
    signal(SIGCHLD,client_reaper);
    signal(SIGENDCLOCK,endclock);
    if(argc<5){
        fprintf(stdout,"usage : %s <port> <client_number> <total_query> <-soption>\n",argv[0]);
        exit(0);
    }
    
    port=argv[1];
    client_n=argv[2];
    total_order=argv[3];
    server_ipaddr=getserveripaddr();

    if(!strcmp(argv[4],"-e")){
        SERVER_ELF_PATH=E_SERVER_ELF_PATH;
    }else if(!strncmp(argv[4],"-t",2)){
        SERVER_ELF_PATH=T_SERVER_ELF_PATH;
        if(argv[4]+2) thread_n=argv[4]+2;
    }
    char* serverargv[]={SERVER_ELF_PATH,port,"BENCHMARK",thread_n,NULL};
    char *clientargv[]={CLIENT_ELF_PATH,server_ipaddr,port,client_n,total_order,NULL,NULL};

    StartPhase("Server Booting");
    {   
        pid_server=fork_and_exec(serverargv);
        sigsuspend(&ss);
        print_execution_time(start);
    }

    StartPhase("Client Throughput Test : Random Mixed");
    {
        pid_client=fork_and_exec(clientargv);
        sigsuspend(&ss);
        print_execution_time(start);
    }
    
    StartPhase("Client Throughput Test : show")
    {   
        serverargv[4]="show";
        pid_client=fork_and_exec(clientargv);
        sigsuspend(&ss);
        print_execution_time(start);
    }

    StartPhase("Client Throughput Test : modify")
    {
        serverargv[4]="modify";
        pid_client=fork_and_exec(clientargv);
        sigsuspend(&ss);
        print_execution_time(start);
    }
    // StartPhase("Client Query Test : Modify");

    // StartPhase("Client Query Test : 50%% show 50%% buy(sell)");

    // StartPhase("Client Query Test : 30%% show 70%% buy(sell)");
    
    // StartPhase("Client Query Test : 70%% show 30%% buy(sell)");

    // StartPhase("Client Exiting");

    kill(pid_server,SIGKILL);
}