#ifndef IMPL_H_
#define IMPL_H_
#include "common.h"
#include "binary_tree.h"
#include "threading.h"
#define NOARGS 0x0
#define INTINT 0x1


extern struct connection connection_frame[];


STATUS show(struct connection* cnct);

STATUS sell(struct connection* cnct);

STATUS buy(struct connection* cnct);

STATUS exit_client(struct connection* cnct);
#endif