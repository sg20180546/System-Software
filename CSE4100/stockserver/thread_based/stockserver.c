#include "stockserver.h"

int main(int argc,char** argv){
    signal(SIGPIPE,SIG_IGN);
    signal(SIGSYNC,sigsync_handler);

    if(argc<2){
        error_exit("usage : ./stockserver {port} mode\n");
    } 
    read_stockfile();
    listenfd=open_listenfd(argv[1]);

    for(i=0;i<NETWORK_WORKER_THREAD_N;i++) 
        pthread_create(&nework_worker_thread_tid[i],NULL,network_worker,NULL);
    clientlen=sizeof(clientaddr);
    mutex_init(&sbuf,PENDING_CONNECTION_N);
    
    mode=PRODUCTION;
    if(argv[2]){
        if(!strcmp(argv[2],"BENCHMARK") || !strcmp(argv[2],"TEST") ) mode=TEST;
    }

    
    while(1){
        if(mode==TEST){ // RUN AT BENCHMARK, ONLY ONCE
            kill(getppid(),SIGUSR1);
            mode=TEST_AT_ONCE;
        }
        connfd=accept(listenfd,(SA*)&clientaddr,&clientlen);
        sbuf_insert(&sbuf,connfd,clientaddr);
    }
    return 0;
}