#ifndef IMPL_H_
#define IMPL_H_
#include "common.h"
#include "binary_tree.h"
#include "network.h"
#define NOARGS 0x0
#define CHARP 0x1
#define INTINT 0x2

struct command{
    int flag;
    char name[COMMAND_N][10];
    char args[3][16];
    union {
        STATUS (*fp0)();
        STATUS (*fp1)(char*);
        STATUS (*fp2)(int,int);
    };
};

STATUS show(char* buf);

STATUS sell(int id, int count);

STATUS buy(int id, int count);

STATUS exit_client(int connfd,int idx);

extern struct command command_list[];

#endif