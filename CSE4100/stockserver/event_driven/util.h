#include "common.h"
#ifndef UTIL_H_
#define UTIL_H_

typedef struct _rio_t{
    int rio_fd;
    int rio_cnt;
    char* rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
} rio_t;


void rio_readinitb(rio_t* rp,int fd);
void Rio_writen(int fd, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp,void* usrbuf,size_t maxlen);
ssize_t rio_read(rio_t* rp,char*usrbuf,size_t n);
int open_listenfd(char* port);
int socket_close(int connfd);

#endif