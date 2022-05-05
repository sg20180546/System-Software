#include "util.h"

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
