#include "stockserver.h"



int main(int argc,char** argv){
    signal(SIGPIPE,SIG_IGN);
    
    if(argc<2) error_exit("usage : ./stockserver {port} mode\n");
    read_stockfile();

    listenfd=open_listenfd(argv[1]);
    init_pool(listenfd);


    if(argv[2]){
          if(!strcmp(argv[2],"BENCHMARK") || !strcmp(argv[2],"TEST") ){
            mode=BENCHMARK; 
            close(STDOUT_FILENO);
            kill(getppid(),SIGSERVERBOOTED);
        }else{
            mode=PRODUCTION;
        }
    }

    while(1){
        see_pool();
        write_pool();
        if(_pool.n==0){
            fsync_stockfile();
        }

    }

    return 0;
}


