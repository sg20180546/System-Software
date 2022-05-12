#ifndef IMPL_H_
#define IMPL_H_
#include "common.h"
#include "binary_tree.h"
#include "network.h"
#define NOARGS 0x0
#define INTINT 0x1

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

STATUS show(struct command* cmd);

STATUS sell(struct command* cmd);

STATUS buy(struct command* cmd);

STATUS exit_client(struct command* cmd);

extern struct command command_list[];

#endif