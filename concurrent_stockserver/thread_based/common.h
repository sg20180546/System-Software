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
#include <stdarg.h>
#include <signal.h>
#include <assert.h>
#include <sys/time.h>
#define LISTENQ  1024
#define RIO_BUFSIZE 8192
#define PENDING_CONNECTION_N 4096
#define NETWORK_WORKER_THREAD_N_DEFAULT 4
#define MAXLINE 8192
#define MAX_STOCK_N 100
#define MAX_FSYNC_TIME 16
#define COMMAND_N 4
#define STOCK_FILE_PATH "./stock.txt"
#define SPACE ' '
#define ENTER '\n'
#define TAB '\t'
#define error_exit(msg) {\
                         fprintf(stderr,(msg));\
                         exit(0);\
                         }
#define free(x) {\
                    if(x) free(x); }
#define whitespace(x) ( ( (x) ==SPACE) || ( (x) == TAB)  )

#define ff(x) fprintf(stderr,"%s\n",x)


typedef struct sockaddr SA;
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

struct connection{
    int connfd;
    char* clienthostname;
    char* clientport;
    int flag;
    char name[10];
    char result[MAXLINE];
    char** args;
    int argc;
    STATUS (*fp)(struct connection*);
};

struct timeval last_fsync_time,cur_time;
FILE* fp;
struct stock* _root;
struct sockaddr_storage clientaddr;
socklen_t clientlen;
int listenfd;
int connfd;
int i;

int NETWORK_WORKER_THREAD_N; 

pthread_t* nework_worker_thread_tid;

// Environment Variable
#define ONLY_ONCE 0x0
#define BENCHMARK 0x1
#define PRODUCTION 0x2

int mode;//default : PRODUCTION

#define SIGSERVERBOOTED SIGUSR1

#endif