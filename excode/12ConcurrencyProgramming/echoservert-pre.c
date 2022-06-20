#include "../csapp.h"
#include "sbuf.h"
#define NTHREADS 4
#define SBUFSIZE 16

static void init_echo_cnt(void);
void echo_cnt(int connfd);
void* thread(void*vargp);

sbuf_t sbuf;
static int byte_cnt;
static sem_t mutex;


int main(int argc,char** argv){
    int i,listenfd,connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;
    if(argc!=2) exit(0);


    listenfd=Open_listenfd(argv[1]);

    sbuf_init(&sbuf,SBUFSIZE);
    for(i=0;i<NTHREADS;i++) Pthread_create(&tid,NULL,thread,NULL);

    while(1){
        clientlen=sizeof(struct sockaddr_storage);
        connfd=Accept(listen,(SA*)&clientaddr,&clientlen);
        sbuf_innsert(&sbuf,connfd);
    }
}

void* thread(void* vargp){
    Pthread_detach(pthread_self());
    while(1){
        int connfd=sbuf_remove(&sbuf);
        echo_cnt(connfd);
        Close(connfd);
    }
}

static void init_echo_cnt(void){
    Sem_init(&mutex,0,1); //?a
    byte_cnt=0;
}
void echo_cnt(int connfd){
    int n;
    char buf[MAXLINE];
    rio_t rio;
    static pthread_once_t once=PTHREAD_ONCE_INIT;
    Pthread_once(&once,init_echo_cnt);
    Rio_readinitb(&rio,connfd);
    while((n=Rio_readlineb(&rio,buf,MAXLINE))!=0 ){
        P(&mutex);
        byte_cnt+=n;
        printf("server received %d (%d total) bytes on fd %d\n",n,byte_cnt,connfd);
        V(&mutex);
        Rio_writen(connfd,buf,n);
    }
}
// 1049