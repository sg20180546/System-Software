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

#define MAX_FSYNC_TIME 16
#define SPACE ' '
#define ENTER '\n'
#define TAB '\t'
#define MAX_STOCK 16
#define MAXLINE 1024
#define STOCK_FILE_PATH "../stock.txt"
#define RIO_BUFSIZE 8192
#define COMMAND_N 4
#define error_exit(msg) {\
                         fprintf(stderr,(msg));\
                         exit(0);\
                         }
#define whitespace(x) ( ( (x) ==SPACE) || ( (x) == TAB)  )


struct stock{
    int id;
    int price;
    int count;
    struct stock* left;
    struct stock* right;
};

typedef enum _STATUS{
    SUCCESS,ERROR
}STATUS;

FILE* fp;
STATUS status;
struct stock* _root;
clock_t time1;
clock_t time2;

#endif