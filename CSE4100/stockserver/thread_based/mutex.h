#ifndef MUTEX_H_
#define MUTEX_H_
#include "common.h"

typedef struct{
    int *buf;
    int n;
    int front,rear;
    sem_t mutex;
    sem_t empty_slots;
    sem_t items;
} sbuf_t;

typedef struct{

} queue;

void mutex_init(sbuf_t*sp,int n);
void sbuf_deinit(sbuf_t*sp);
void sbuf_insert(sbuf_t* sp,int item);
int sbuf_remove(sbuf_t* sp);
void* get_fd(void* vargp);

sem_t writer_n_mutex;
sem_t writer_prior_lock; // binary

sem_t reader_n_mutex;
int reader_n;
int writer_n;

sbuf_t sbuf;

int cur_connection=0;

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