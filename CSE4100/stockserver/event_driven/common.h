#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <netinet/tcp.h>
#include <wait.h>

#define MAX_FSYNC_TIME 16
#define SPACE ' '
#define ENTER '\n'
#define TAB '\t'
#define MAX_STOCK 16
#define MAXLINE 8192
#define STOCK_FILE_PATH "./stock.txt"
#define RIO_BUFSIZE 8192
#define COMMAND_N 4
#define LISTENQ 1024
#define error_exit(msg) {\
                         fprintf(stderr,(msg));\
                         exit(0);\
                         }
#define whitespace(x) ( ( (x) ==SPACE) || ( (x) == TAB)  )

#define free(x) {\
                    if(x) free(x); }

typedef struct sockaddr SA;
struct stock{
    int id;
    int price;
    int count;
    struct stock* left;
    struct stock* right;
};
typedef enum _STATUS{
    SUCCESS,ERROR,NOCMD,INVARG,NOTENOUGHERR,NL
}STATUS;
struct command{
    int flag;
    int connfd;
    int poolidx;
    char name[10];
    char result[MAXLINE];
    char** args;
    int argc;
    STATUS (*fp)(struct command*);
};



FILE* fp;
struct stock* _root;
clock_t time1;
clock_t time2;

#endif