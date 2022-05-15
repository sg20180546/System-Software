#include "common_test.h"
#include "../stockserver.h"

static int fork_and_exec(char* path,char** argv){
    pid_t pid=fork();
    if(pid==0){
        execl(path,argv);
    }
}
void reaper(){
    while(waitpid(0,NULL,WNOHANG));
}

int main(int argc,char** argv){
    // ./stockserver_test {port} {clientnumber};
    signal(SIGCHLD,reaper);
    if(argc!=3){
        fprintf(stdout,"usage : %s {port} {clientnumber}",argv[0]);
    }
    StartPhase("Server Booting");
    {
        fork_and_exec("../stockserver",argv[1]);
    }

    StartPhase("Client Connection Test");
    {
        int i;
        for(i=0;i<atoi(argv[2]);i++) fork_and_exec("../cleint/stockclient","127.0.0.1 {port}");
    
    }

    StartPhase("Client Query Test : show");

    StartPhase("Client Query Test : buy(sell)");

    StartPhase("Client Query Test : 50%% show 50%% buy(sell)");

    StartPhase("Client Query Test : 30%% show 70%% buy(sell)");
    
    StartPhase("Client Query Test : 70%% show 30%% buy(sell)");



    StartPhase("Client Exiting");
}