#include "stockserver.h"

int main(int argc,char** argv){
    
    if(argc!=2) exit(0);

    read_stockfile();


    listenfd=open_listenfd(argv[1]);

    for(i=0;i<NTHREADS;i++) pthread_create(&tid,NULL,get_fd,NULL);
    clientlen=sizeof(clientaddr);
    mutex_init(&sbuf,NTHREADS);



    while(1){
        connfd=accept(listenfd,(SA*)&clientaddr,&clientlen);
        sbuf_insert(&sbuf,connfd);

        if(cur_connection==0){
            fsync_stockfile();
        }
    }

    

}