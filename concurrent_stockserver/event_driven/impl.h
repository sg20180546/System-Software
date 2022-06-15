#ifndef IMPL_H_
#define IMPL_H_
#include "common.h"
#include "binary_tree.h"
#include "network.h"
#define NOARGS 0x0
#define INTINT 0x1



STATUS show(struct command* cmd);

STATUS sell(struct command* cmd);

STATUS buy(struct command* cmd);

STATUS exit_client(struct command* cmd);

extern struct command command_list[];

#endif