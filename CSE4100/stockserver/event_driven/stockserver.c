#include "stockserver.h"



int main(int argc,char** argv){
    signal(SIGPIPE,SIG_IGN);
    
    if(argc<2) error_exit("usage : ./stockserver {port} mode\n");

    read_stockfile();
    listenfd=open_listenfd(argv[1]);
    int mode;
    if(argv[2]){
        if(!strcmp(argv[2],"BENCHMARK")) mode=BENCHMARK;
    }
    init_pool(listenfd);
    
    while(1){
        if(mode==BENCHMARK){ // RUN AT BENCHMARK, ONLY ONCE
            kill(getppid(),SIGUSR1);
            mode=ONLY_ONCE;
        }
        see_pool();
        write_pool();
        if(_pool.n==0||time_check()>MAX_FSYNC_TIME){
            fsync_stockfile();
        }

    }

    return 0;
}


