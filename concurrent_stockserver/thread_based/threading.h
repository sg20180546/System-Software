#ifndef THREADING_H_
#define THREADING_H_
#include "common.h"
#include "network.h"
#include "stockfile_handle.h"







typedef struct _waiting_connfd {
    int connfd;
    struct sockaddr_storage clientaddr;
}waiting_connfd;

typedef struct{
    waiting_connfd *waiting_connfd;
    int n;
    int front,rear;
    sem_t mutex;
    sem_t empty_slots;
    sem_t items;
} sbuf_t;

void mutex_init(sbuf_t*sp,int n);
void sbuf_deinit(sbuf_t*sp);
void sbuf_insert(sbuf_t* sp,int item,struct sockaddr_storage ss);
waiting_connfd sbuf_remove(sbuf_t* sp);
void* network_worker(void* vargp);
void sigsync_handler(int sig);


sem_t writer_n_mutex;
sem_t reader_n_mutex;
sem_t fsync_worker_mutex;
int reader_n;
int writer_n;

sem_t idle_threads;

sbuf_t sbuf;


// sem_t writer_mutex; go to in the struct stock
// sem_t reader_n;
// watching reader_n, if reader_n==0 , "only" writer get mutex
// watching writer_n, if writer_n==0, reader can get mutex
// if there is writer is in queue, readers are inprior to writers
// == watcing writer_q , is there something, thread_worker do not work on read for on
// -> after all reader thread done, writers queue will all be on job while empty
// reader writers problem
/*
 0.if(reader_come){
    if(writer_n>0){
    push(tid)
    suspend()
    }
    
    do job();
 }
 1.if(writer come){
     if(reader_n>0){
         push(tid)
         suspend()
     }
     
 }
 
 2.while(writing) readers wait

 3.
*/
#endif