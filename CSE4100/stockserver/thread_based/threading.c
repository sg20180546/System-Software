#include "threading.h"


void* network_worker(void* vargp){
    pthread_detach(pthread_self());
    int idle_thread_n;
    while(1){
        waiting_connfd wc=sbuf_remove(&sbuf);
        // printf("%d\n",wc.connfd);
        sem_wait(&idle_threads);
        char clienthostname[MAXLINE],clientport[MAXLINE];
        getnameinfo((SA*)&wc.clientaddr,sizeof(wc.clientaddr),clienthostname,MAXLINE,clientport,MAXLINE,0);
        thread_safe_printf("Connected to (%s:%s)\n",clienthostname,clientport);
        service(wc.connfd,clienthostname,clientport);
        sem_post(&idle_threads);
        
        sem_getvalue(&idle_threads,&idle_thread_n);
        if(idle_thread_n==NETWORK_WORKER_THREAD_N){
            sem_wait(&sbuf.mutex);
            kill(getpid(),SIGSYNC);
        } 
    }
    return NULL;
}

void sigsync_handler(int sig){
    fsync_stockfile();
    sem_post(&sbuf.mutex);
}



void mutex_init(sbuf_t * sp,int n){
    nework_worker_thread_tid=malloc(sizeof(pthread_t)*NETWORK_WORKER_THREAD_N);
    writer_n=0;
    reader_n=0;
    sem_init(&reader_n_mutex,0,1);
    sem_init(&writer_n_mutex,0,1);
    sem_init(&fsync_worker_mutex,0,0);
    sem_init(&idle_threads,0,NETWORK_WORKER_THREAD_N);
    sp->waiting_connfd=calloc(n,sizeof(waiting_connfd));
    sp->n=n;
    sp->front=sp->rear=0;
    sem_init(&sp->mutex,0,1);
    sem_init(&sp->empty_slots,0,n);
    sem_init(&sp->items,0,0);

}

void sbuf_deinit(sbuf_t*sp){
    free(sp->waiting_connfd);
}
void sbuf_insert(sbuf_t*sp,int item,struct sockaddr_storage ss){
    sem_wait(&sp->empty_slots);
    sem_wait(&sp->mutex);
    sp->waiting_connfd[(++sp->rear%(sp->n))].connfd=item;
    sp->waiting_connfd[(sp->rear%(sp->n))].clientaddr=ss;
    sem_post(&sp->mutex);
    // printf("inserting?");
    sem_post(&sp->items);
}

waiting_connfd sbuf_remove(sbuf_t* sp){
    waiting_connfd wc;
    // int t;
    // sem_getvalue(&sp->items,&t);
    // printf("t: %d\n ",t);
    sem_wait(&sp->items);
    sem_wait(&sp->mutex);
    wc.connfd=sp->waiting_connfd[(++sp->front)%(sp->n)].connfd;
    wc.clientaddr=sp->waiting_connfd[(sp->front)%(sp->n)].clientaddr;
    sem_post(&sp->mutex);
    sem_post(&sp->empty_slots);
    return wc;
}