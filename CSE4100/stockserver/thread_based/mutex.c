#include "mutex.h"


void* get_fd(void* vargp){
    pthread_detach(pthread_self());
    while(1){
        int thread_connfd=sbuf_remove(&sbuf);
        do_task(thread_connfd);
        //after connection close, cur_Connection--;
    }
}


void mutex_init(sbuf_t * sp,int n){
    writer_n=0;
    reader_n=0;
    sem_init(&reader_n_mutex,0,1);
    sem_init(&writer_n_mutex,0,1);
    sp->buf=Calloc(n,sizeof(int));
    sp->n=n;
    sp->front=sp->rear=0;
    sem_init(&sp->mutex,0,1);
    sem_init(&sp->empty_slots,0,n);
    sem_init(&sp->items,0,0);
}

void sbuf_deinit(sbuf_t*sp){
    free(sp->buf);
}
void sbuf_insert(sbuf_t*sp,int item){
    sem_wait(&sp->empty_slots);
    sem_wait(&sp->mutex);
    sp->buf[(++sp->rear%(sp->n))]=item;
    sem_post(&sp->mutex);
    sem_post(&sp->items);
}

int sbuf_remove(sbuf_t * sp){
    int item;
    sem_wait(&sp->items);
    sem_wait(&sp->mutex);
    item=sp->buf[(++sp->front)%(sp->n)];
    sem_post(&sp->mutex);
    sem_post(&sp->empty_slots);
    return item;
}