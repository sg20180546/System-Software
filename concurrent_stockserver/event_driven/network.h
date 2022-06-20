#ifndef NETWORK_H_
#define NETWORK_H_
#include "common.h"
#include "impl.h"
#include "util.h"
#include "parser.h"



typedef struct{
    int maxfd;
    fd_set read_set;
    fd_set ready_set;
    int nready;
    int n;
    int maxi;
    int clientfd[FD_SETSIZE];
    rio_t clientrio[FD_SETSIZE];
}pool;


int listenfd;
socklen_t clientlen;
struct sockaddr_storage clientaddr;
pool _pool;

void init_pool(int listenfd);
void see_pool(void);
void write_pool(void);
int remove_client(int connfd,int idx);

#endif