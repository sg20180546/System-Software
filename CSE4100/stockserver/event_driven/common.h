#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FSYNC_TIME 60
#define MAX_STOCK 16
#define MAXLINE 1024
#define STOCK_FILE_PATH "../stock.txt"

#define error_exit(msg) {\
                         fprintf(stderr,(msg));\
                         exit(0);\
                         }





struct stock{
    int id;
    int price;
    int count;
    struct stock* left;
    struct stock* right;
};

typedef enum{
    SUCCESS,ERROR
}STATUS;

FILE* fp;
STATUS status;
struct stock* root;

#endif