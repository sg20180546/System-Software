#ifndef NETWORK_H_
#define NETWORK_H_
#include "common.h"
#include "impl.h"
#define LISTENQ 1024


typedef struct{
    int rio_fd;
    int rio_cnt;
    char* rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
} rio_t;

typedef struct{
    int maxfd;
    fd_set read_set;
    fd_set ready_set;
    int nready;
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
ssize_t rio_readlineb(rio_t*rp,void *usrbuf,size_t maxlen);
ssize_t rio_writen(int fd,void* usrbuf,size_t n);
#endif