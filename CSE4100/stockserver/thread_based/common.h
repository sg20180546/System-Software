#ifndef COMMON_H_
#define COMMON_H_
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#define LISTENQ  1024
#define RIO_BUFSIZE 8192
#define NTHREADS 4
#define MAX_STOCK_N 100 
int listenfd;

struct stock{
    int id;
    int price;
    int count;
    struct stock* left;
    struct stock* right;
    sem_t modify_mutex;// modifying ,not in show
};
#endif