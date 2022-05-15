#include "stockserver.h"

int main(int argc,char** argv){
    
    if(argc!=2) exit(0);

    read_stockfile();

    listenfd=open_listenfd(argv[1]);
    pthread_create(&fsync_worker_thread_tid,NULL,fsync_worker,NULL);
    for(i=0;i<NETWORK_WORKER_THREAD_N;i++) 
        pthread_create(&nework_worker_thread_tid[i],NULL,network_worker,NULL);
    
    clientlen=sizeof(clientaddr);
    mutex_init(&sbuf,PENDING_CONNECTION_N);



    while(1){
        connfd=accept(listenfd,(SA*)&clientaddr,&clientlen);
        sbuf_insert(&sbuf,connfd,clientaddr);
    }
    return 0;
}