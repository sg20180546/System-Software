#include "../csapp.h"
#define MAXTHREADS 32

void* sum_mutex(void* vargp);
void* sum_array(void* vargp);
void* sum_local(void* vargp);

long gsum=0;
long nelems_per_threads,psum[MAXTHREADS];
sem_t mutex;

int main(int argc,char** argv){
    clock_t start=clock();
    long i,nelems,long_nelems,nthreads,myid[MAXTHREADS];
    pthread_t tid[MAXTHREADS];

    if(argc!=3) exit(0);
    nthreads=atoi(argv[1]);
    long_nelems=atoi(argv[2]);

    nelems=(1L<<long_nelems);
    printf("nelems: %ld\n",nelems);
    nelems_per_threads=nelems/nthreads;
    sem_init(&mutex,0,1);

    for(i=0;i<nthreads;i++){
        myid[i]=i;
        Pthread_create(&tid[i],NULL,sum_local,&myid[i]);
    }
    for(i=0;i<nthreads;i++) Pthread_join(tid[i],NULL);

    for(i=0;i<nthreads;i++) gsum+=psum[i];

    if(gsum!=(nelems*(nelems-1))/2 ) printf("Error: result=%ld\n",gsum);
    clock_t end=clock();
    printf("execution time : %ld\n",end-start);
    exit(0);
}

void* sum_mutex(void*vargp){
    long myid=*((long*)vargp);
    long start=myid*nelems_per_threads;
    long end=start+nelems_per_threads;
    long i;
    for(i=start;i<end;i++){
        P(&mutex);
        gsum+=i;
        V(&mutex);
    }
    return NULL;
}

void *sum_array(void*vargp){
    long i;
    long myid=*((long*)vargp);
    long start=myid*nelems_per_threads;
    long end=start+nelems_per_threads;

    for(i=start;i<end;i++) psum[myid]+=i;
    return NULL;
}
void *sum_local(void *vargp){
    long myid=*((long*)vargp);
    long start=myid*nelems_per_threads;
    long end=start+nelems_per_threads;
    long i,sum=0;
    for(i=start;i<end;i++){
        sum+=i;
    }
    psum[myid]=sum;
}