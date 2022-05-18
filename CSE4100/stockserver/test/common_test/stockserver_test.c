#include "common_test.h"

static pid_t fork_and_exec(char** argv){
    pid_t pid=fork();
    if(pid==0){
        if(execv(argv[0],argv)<0){
            write(1,"exec failed",12);
            exit(0);
        }
    }else if(strstr(argv[1],"client")) pid_client[cur_client_n++]=pid;

    return pid;
}

static void print_execution_time(const char* tp,clock_t s,clock_t e){
    printf("Execution time : %.lf millisecond\n",(double)e-s);
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

void reaper(int sig){
    while(waitpid(0,NULL,WNOHANG)){
        cur_client_n--;
        if(cur_client_n==0){
            kill(SIGENDCLOCK,getpid());
        }
    }
}

void endclock(int sig){
    end=clock();
    kill(getpid(),SIGCONT);
}

int main(int argc,char** argv){
        // printf("%d",argc);
    clock_t start,end;
    sigset_t ss;
    char* port,*server_ipaddr,*client_n;
    char* SERVER_ELF_PATH;
    signal(SIGCHLD,reaper);
    signal(SIGENDCLOCK,endclock);

    if(argc<2){
        fprintf(stdout,"usage : %s {port} {clientnumber} -{soption}\n",argv[0]);
        exit(0);
    }
    
    port=argv[1];
    client_n=argv[2];
    server_ipaddr=getserveripaddr();

    if(!strcmp(argv[3],"-e")){
        SERVER_ELF_PATH=E_SERVER_ELF_PATH;
    }else{
        SERVER_ELF_PATH=T_SERVER_ELF_PATH;
    }
    char* serverargv[]={SERVER_ELF_PATH,port,"BENCHMARK",NULL};
    // char *clientargv[]={CLIENT_ELF_PATH,server_ipaddr,port,client_n,NULL};
    assert(!strcmp("127.0.0.1",server_ipaddr));
    assert(!strcmp("1024",port));
    char *clientargv[]={CLIENT_ELF_PATH,server_ipaddr,port,client_n,NULL};
    // for(int i=0;i<4;i++) printf("%s\n",clientargv[i]);
    StartPhase("Server Booting");
    {   
        pid_server=fork_and_exec(serverargv);
        sigsuspend(&ss);
        print_execution_time(phase,start,end);
    }

    StartPhase("Client Connection Test");
    {
        fork_and_exec(clientargv);
        sigsuspend(&ss);
        print_execution_time(phase,start,end);
    }

    // StartPhase("Singular Client Query Test : show");
    // {
        
    // }
    // StartPhase("Singular Client Query Test : buy(sell)");
    // {

    // }
    // StartPhase("Singular Client Query Test : 50%% show 50%% buy(sell)");
    // {
    //     // fork_and_exec client
    //     // sleeping
    //     // if all client exit, sigendclock comes
    //     // print execution time
    // }
    // StartPhase("Singular Client Query Test : 30%% show 70%% buy(sell)");
    // {

    // }
    // StartPhase("Singular Client Query Test : 70%% show 30%% buy(sell)");
    // {

    // }
    
    // StartPhase("Multiple Client Query Test : show");

    // StartPhase("Multiple Client Query Test : buy(sell)");

    // StartPhase("Multiple Client Query Test : 50%% show 50%% buy(sell)");

    // StartPhase("Multiple Client Query Test : 30%% show 70%% buy(sell)");
    
    // StartPhase("Multiple Client Query Test : 70%% show 30%% buy(sell)");

    // StartPhase("Client Exiting");

    kill(pid_server,SIGKILL);
}