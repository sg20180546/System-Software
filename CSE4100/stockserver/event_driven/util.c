#include "util.h"
static ssize_t rio_writen(int fd, void *usrbuf, size_t n);
static void unix_error(char *msg);

static void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

static ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
	if ((nwritten = write(fd, bufp, nleft)) <= 0) {
	    if (errno == EINTR)  /* Interrupted by sig handler return */
		nwritten = 0;    /* and call write() again */
	    else
		return -1;       /* errno set by write() */
	}
	nleft -= nwritten;
	bufp += nwritten;
    }
    return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n)
{
    if (rio_writen(fd, usrbuf, n) != n)
	unix_error("Rio_writen error");
}


void rio_readinitb(rio_t* rp,int fd){
    rp->rio_fd=fd;
    rp->rio_cnt=0;
    rp->rio_bufptr=rp->rio_buf;
}

ssize_t rio_readlineb(rio_t* rp,void* usrbuf,size_t maxlen){
    int n,rc;
    char c,*bufp=usrbuf;
    for(n=1;n<maxlen;n++){
        if((rc=rio_read(rp,&c,1))==1 ){
            *bufp=c;
            bufp++;
            if(c=='\n'){
                n++;
                break;
            }
        }else if(rc==0){
            if(n==1) return 0;
            else break;
        }else return -1;
    }
    *bufp=0;
    return n-1;
}

ssize_t rio_read(rio_t* rp,char*usrbuf,size_t n){
    int cnt;
    while(rp->rio_cnt<=0){
        rp->rio_cnt=read(rp->rio_fd,rp->rio_buf,sizeof(rp->rio_buf));
        if(rp->rio_cnt<0){
            if(errno!=EINTR) return -1;
        }else if(rp->rio_cnt==0) return 0;
        else rp->rio_bufptr=rp->rio_buf;
    }
    cnt=n;
    if(rp->rio_cnt<n) cnt=rp->rio_cnt;
    memcpy(usrbuf,rp->rio_bufptr,cnt);
    rp->rio_bufptr+=cnt;
    rp->rio_cnt-=cnt;
    return cnt;
}

int open_listenfd(char* port){
    struct addrinfo hints,*listp,*p;
    int listenfd,optval=1;
    struct timeval t;
    t.tv_sec=t.tv_usec=0;
    struct linger l;

    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE| AI_ADDRCONFIG;
    hints.ai_flags|=AI_NUMERICSERV;
    getaddrinfo(NULL,port,&hints,&listp);

    for(p=listp;p;p=p->ai_next){
        if((listenfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0) continue;

        setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(int));
        // setsockopt(listenfd,SOL_SOCKET,SO_SNDTIMEO,(const void*)&t,sizeof(t));
        setsockopt(listenfd,SOL_SOCKET,SO_RCVTIMEO,(const void*)&t,sizeof(t));
        if(bind(listenfd,p->ai_addr,p->ai_addrlen)==0) break;
        close(listenfd);
    }
    freeaddrinfo(listp);
    if(!p) return -1;

    if(listen(listenfd,LISTENQ)<0) {
        close(listenfd);
        return -1;
    }
    printf("Server running on %s\n",port);
    return listenfd;
}

// int open_listenfd(char* port) 
// {
//   int listenfd, optval=1;
//   struct sockaddr_in serveraddr;
  
//   /* Create a socket descriptor */
//   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//     return -1;
 
//   /* Eliminates "Address already in use" error from bind. */
//   if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
//                  (const void *)&optval , sizeof(int)) < 0)
//     return -1;

//   /* Listenfd will be an endpoint for all requests to port
//      on any IP address for this host */
//   bzero((char *) &serveraddr, sizeof(serveraddr));
//   serveraddr.sin_family = AF_INET; 
//   serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
//   serveraddr.sin_port = port; 

//   if (bind(listenfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0)
//     return -1;

//   /* Make it a listening socket ready to accept connection requests */
//   if (listen(listenfd, LISTENQ) < 0)
//     return -1;

//   return listenfd;
// }
int socket_close(int connfd){
    int st;
    if((st=close(connfd))<0) printf("Connection Closed failed\n");
    else printf("Connection Closed\n");
    return st;
}