#include "util.h"
static void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
ssize_t rio_writen(int connfd,char* usrbuf,size_t n){
    size_t nleft=n;
    ssize_t nwritten;
    char* bufp=usrbuf;
    while(nleft>0){
        if((nwritten=write(connfd,bufp,nleft))<=0 ){
            if(errno==EINTR) nwritten=0;
            else return -1;

        }
        nleft-=nwritten;
        bufp+=nwritten;
    }
    return (ssize_t)n;
}

void Rio_writen(int fd, void *usrbuf)
{
    if (rio_writen(fd, usrbuf, MAXLINE) != MAXLINE) unix_error("Rio_writen error");
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

    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE| AI_ADDRCONFIG;
    hints.ai_flags|=AI_NUMERICSERV;
    getaddrinfo(NULL,port,&hints,&listp);

    for(p=listp;p;p=p->ai_next){
        if((listenfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0) continue;

        setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(int));
        if(bind(listenfd,p->ai_addr,p->ai_addrlen)==0) break;
        close(listenfd);
    }
    freeaddrinfo(listp);
    if(p==NULL) return -1;

    if(listen(listenfd,LISTENQ)<0) {
        close(listenfd);
        return -1;
    }
    printf("Server running on %s\n",port);
    return listenfd;
}

int socket_close(int connfd){
    int st;
    if((st=close(connfd))<0) thread_safe_printf("Connection Closed failed\n");
    else thread_safe_printf("Connection Closed\n");
    return st;
}

void thread_safe_printf(const char* format,...){
    char buf[MAXLINE];
    va_list arglist;
    va_start(arglist,format);
    vsprintf(buf,format,arglist);
    va_end(arglist);
    write(1,buf,strlen(buf));
}

void print_sem_value(sem_t* s){
    int n;
    char res[64];
    sem_getvalue(s,&n);
    sprintf(res,"sem value :%d\n",n);
    write(1,res,strlen(res));
    return;
}