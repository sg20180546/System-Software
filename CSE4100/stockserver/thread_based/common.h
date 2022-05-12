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
#define MAXLINE 1024
#define MAX_STOCK_N 100
#define MAX_FSYNC_TIME 16
#define STOCK_FILE_PATH "./stock.txt"

typedef struct sockaddr SA;
int listenfd;
int connfd;
int i;
pthread_t tid;
struct stock{
    int id;
    int price;
    int count;
    struct stock* left;
    struct stock* right;
    sem_t modify_mutex;// modifying ,not in show , binary mutex
};
typedef enum _STATUS{
    SUCCESS,ERROR,NOCMD,INVARG,NOTENOUGHERR,NL
}STATUS;
struct stock* _root;
struct sockaddr_storage clientaddr;
socklen_t clientlen;
FILE* fp;
#endif