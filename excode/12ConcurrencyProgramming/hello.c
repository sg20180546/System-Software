#include "csapp.h"

void *thread(void *vargp){
    printf("Hello, world\n");
    return NULL;
}

int main(){
    pthread_t tid;
    // int pthread_create(pthread_t* tid, pthread_attr_t *attr, func* f,void * arg);
    Pthread_create(&tid,NULL,thread,NULL);
    Pthread_join(tid,NULL);
    exit(0);
}