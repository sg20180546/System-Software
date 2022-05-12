#ifndef IMPL_H_
#define IMPL_H_
#include "common.h"
#include "binary_tree.h"
#include "mutex.h"
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

extern struct command command_list[];


STATUS show(struct command* cmd);

STATUS sell(struct command* cmd);

STATUS buy(struct command* cmd);

STATUS exit_client(struct command* cmd);
#endif