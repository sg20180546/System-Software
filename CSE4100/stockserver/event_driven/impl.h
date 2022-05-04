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
    char name[3][10];
    union {
        STATUS (*fp0)();
        STATUS (*fp1)(char*);
        STATUS (*fp2)(int,int);
    };
};

STATUS show(char* buf);

STATUS sell(int id, int count);

STATUS buy(int id, int count);

STATUS exit_client();

struct command command_list[COMMAND_N]={
    {.flag=0x1,.name={"show","SHOW","Show"},.fp1=show},
    {.flag=0x2,.name={"sell","SELL","Sell"},.fp2=sell},
    {.flag=0x2,.name={"buy","BUY","Buy"},.fp2=buy},
    {.flag=0x0,.name={"exit","EXIT","Exit"},.fp0=exit_client}
}

#endif