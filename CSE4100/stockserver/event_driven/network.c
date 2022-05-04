#include "network.h"

static STATUS find_cmd(char* buf,void* fp);
static STATUS find_args(char* buf,char** args);
static void add_client(connfd);
static void rio_readinitb(rio_t* rp,int fd);
static ssize_t rio_writen(int fd,void* usrbuf,size_t n);
static ssize_t rio_readlineb(rio_t *rp,void* usrbuf,size_t maxlen);


void init_pool(int listenfd){
    int i;
    _pool.maxi=-1;
    for(i=0;i<FD_SETSIZE;i++) _pool.clientfd[i]=-1;
    _pool.maxfd=listenfd;
    FD_ZERO(&_pool.read_set);
    FD_SET(listenfd,&_pool.read_set);
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
    return listenfd;
}

void see_pool(void){
    _pool.ready_set=_pool.read_set;
    _pool.nready=select(_pool.maxfd+1,&_pool.read_set,NULL,NULL,NULL);
    if(FD_ISSET(listenfd,&_pool.ready_set)){
        clientlen=sizeof(struct sockaddr_storage);
        connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen);
        add_client(connfd);
    }
}

void write_pool(void){
    int i,connfd;
    char buf[MAXLINE];

    rio_t rio;

    for(i=0;(i<=_pool.maxi)&&(_pool.nready>0);i++){
        connfd=_pool.clientfd[i];
        rio=_pool.clientrio[i];

        if((connfd>0)&&(FD_ISSET(connfd,&_pool.ready_set))){
            _pool.nready--;
            void* fp=NULL;
            char args[10][10];
            if((connfd>0)&&(FD_ISSET(connfd,&_pool.ready_set))){
                if(rio_readlineb(&rio,buf,MAXLINE)!=0){
                    if(find_cmd(buf,fp)==ERROR){
                        
                        continue;
                    }
                    if(find_args(buf,args)==ERROR){
                        
                        continue;
                    }
                }
            }else{
                close(connfd);
                FD_CLR(connfd,&_pool.read_set);
                _pool.clientfd[i]=-1;
            }
        }

    }
}

static void add_client(int connfd){
    int i;
    _pool.nready--;
    for(i=0;i<FD_SETSIZE;i++){
        if(_pool.clientfd[i]<0){
            _pool.clientfd[i]=connfd;
            rio_readinitb(&_pool.clientrio[i],connfd);
            FD_SET(connfd,&_pool.read_set);
            if(connfd>_pool.maxfd) _pool.maxfd=connfd;
            if(i>_pool.maxi) _pool.maxi=i;
            break;
        }
    }
    if(i==FD_SETSIZE) error_exit("add client error : Too many client\n");
}

static void rio_readinitb(rio_t* rp,int fd){
    rp->rio_fd=fd;
    rp->rio_cnt=0;
    rp->rio_bufptr=rp->rio_buf;
}

ssize_t rio_readlineb(rio_t* rp,void* usrbuf,size_t maxlen){
    int n,rc;
    char c,*bufp=usrbuf;
}

static STATUS find_cmd(char* buf,void* fp){
    int i,j;
    size_t n;
    char* p=buf;
    while(!whitespace(*p)) p++;
    n=p-buf;
    for(i=0;i<COMMAND_N ;i++){
        for(j=0;j<3;j++){
            if(!strncmp(command_list[i].name[j],buf,n)){
                switch (command_list[i].flag)
                {
                case 0x0:
                    /* code */
                    break;
                case 0x1:

                    break;
                case 0x2:

                    break;
                default:
                    break;
                }
            }
        }
    }
    buf=*p;

}