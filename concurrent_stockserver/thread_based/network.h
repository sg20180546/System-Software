#ifndef NETWORK_H_
#define NETWORK_H_
#include "common.h"
#include "util.h"
#include "parser.h"

void service(int connfd,char* hostname,char* port);

#endif