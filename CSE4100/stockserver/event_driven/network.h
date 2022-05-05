#ifndef NETWORK_H_
#define NETWORK_H_
#include "common.h"
#include "impl.h"
#include "util.h"
#include "parser.h"
#define LISTENQ 1024



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


int listenfd,connfd;
socklen_t clientlen;
struct sockaddr_storage clientaddr;
pool _pool;

void init_pool(int listenfd);
int open_listenfd(char* port);
void see_pool(void);
void write_pool(void);
int remove_client(int connfd,int idx);

#endif