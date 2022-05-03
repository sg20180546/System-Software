#ifndef IMPL_H_
#define IMPL_H_
#include "common.h"
#include "binary_tree.h"
STATUS show(int connfd);

STATUS sell(int connfd,int id, int count);

STATUS buy(int connfd,int id, int count);

STATUS exit_client();

void event_driven();

#endif