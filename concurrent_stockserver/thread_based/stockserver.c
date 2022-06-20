#include "stockserver.h"

int main(int argc,char** argv){
    signal(SIGPIPE,SIG_IGN);
    signal(SIGSYNC,sigsync_handler);

    if(argc<2){
        error_exit("usage : ./stockserver {port} {mode} {thread_n}\n");
    } 

    NETWORK_WORKER_THREAD_N=NETWORK_WORKER_THREAD_N_DEFAULT;
    if(argv[3]){
        // printf("ar %s c : %c",argv[3],argv[3][0]);
        int n=atoi(argv[3]);
        if(n) NETWORK_WORKER_THREAD_N=n;
    }
    
    
    mutex_init(&sbuf,PENDING_CONNECTION_N);

    read_stockfile();
    listenfd=open_listenfd(argv[1]);
    for(i=0;i<NETWORK_WORKER_THREAD_N;i++) 
        pthread_create(&nework_worker_thread_tid[i],NULL,network_worker,NULL);
    clientlen=sizeof(clientaddr);



    if(argv[2]){
        if(!strcmp(argv[2],"BENCHMARK") || !strcmp(argv[2],"TEST") ){
            mode=BENCHMARK;
            // close(STDOUT_FILENO);
            kill(getppid(),SIGSERVERBOOTED);
        }else{
            mode=PRODUCTION;
        }
    }
    

    
    while(1){
        connfd=accept(listenfd,(SA*)&clientaddr,&clientlen);
        // printf("%d\n",connfd);
        if(connfd>listenfd) sbuf_insert(&sbuf,connfd,clientaddr);
    }
    return 0;
}