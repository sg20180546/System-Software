#include "network.h"

static STATUS find_cmd(char buf[],struct command* cmd,int* p);
static STATUS find_args(char* buf,struct command* cmd);
static void execute(int connfd,struct command cmd);
static void add_client(int connfd);


static void rio_readinitb(rio_t* rp,int fd);
static ssize_t rio_writen(int fd,void* usrbuf,size_t n);
static ssize_t rio_readlineb(rio_t *rp,void* usrbuf,size_t maxlen);
static ssize_t rio_read(rio_t* rp,char*usrbuf,size_t n);


void init_pool(int listenfd){
    int i;
    _pool.n=0;
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
    _pool.nready=select(_pool.maxfd+1,&_pool.ready_set,NULL,NULL,NULL);
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
            char args[3][16];
            struct command cmd;
            int flag,p=0;
            if((connfd>0)&&(FD_ISSET(connfd,&_pool.ready_set))){
                if(rio_readlineb(&rio,buf,MAXLINE)!=0){
                    if(find_cmd(buf,&cmd,&p)==ERROR){
                        rio_writen(connfd,"no such command\n",20);
                        continue;
                    }

                    if(find_args((buf+p),&cmd)==ERROR){
                        rio_writen(connfd,"invalid argument\n",20);
                        continue;
                    }
                }
            sprintf(cmd.args[0],"%d",i);
            execute(connfd,cmd);
            }else{
                remove_client(connfd,i);
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
    _pool.n++;
}
int remove_client(int connfd,int idx){
    if(close(connfd)<0) return -1;
    FD_CLR(connfd,&_pool.read_set);
    _pool.clientfd[idx]=-1;
    _pool.n--;
    return 1;
}


static void rio_readinitb(rio_t* rp,int fd){
    rp->rio_fd=fd;
    rp->rio_cnt=0;
    rp->rio_bufptr=rp->rio_buf;
}

static ssize_t rio_readlineb(rio_t* rp,void* usrbuf,size_t maxlen){
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

static ssize_t rio_read(rio_t* rp,char*usrbuf,size_t n){
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




static STATUS find_cmd(char buf[],struct command* cmd,int* p){
    int i,j;
    size_t n;
    unsigned int pos=0;
    while(! whitespace( buf[pos] ) ) pos++;
    for(i=0;i<COMMAND_N ;i++){
        for(j=0;j<3;j++){
            if(!strncmp(command_list[i].name[j],(*buf),pos)){
                cmd->flag=command_list[i].flag;
                strcpy(cmd->name[0],command_list[i].name);
                switch (cmd->flag)
                {
                case 0x0:
                    cmd->fp0=command_list[i].fp0;
                    break;
                case 0x1:
                    cmd->fp1=command_list[i].fp1;
                    break;
                case 0x2:
                    cmd->fp2=command_list[i].fp2;
                    break;
                default:
                    break;
                }
            }
        }
    
    if(cmd->fp0==NULL) return ERROR;
    else {*p=pos; return SUCCESS;}
}

static STATUS find_args(char* buf,struct command* cmd){
    unsigned int pos=0;
    switch (cmd->flag)
    {
    case (NOARGS||CHARP):
        while(whitespace(*buf)) pos++;
        if(buf[pos]!=ENTER) return ERROR;
        break;
    case INTINT:
        while(whitespace(*buf)) (*buf)++;
        p=(*buf);
        while(!whitespace(*buf)) (*buf)++;
        strncpy(cmd->args[1],p,(*buf)-p);

        while(whitespace(*buf)) (*buf)++;
        p=(*buf);
        while(!whitespace(*buf)) (*buf)++;
        strncpy(cmd->args[2],p,(*buf)-p);
        
        if(*buf!=ENTER) return ERROR;
        break;
    default:
        break;
    }
    return SUCCESS;
}

static void execute(int connfd,struct command cmd){
    char buf1[MAXLINE]="",buf2[MAXLINE]="[";
    STATUS st;
    strcat(buf2,cmd.name);
    switch (cmd.flag)
    {
    case NOARGS:
        st=cmd.fp0();
        break;
    case CHARP:
        st=cmd.fp1(buf1);
        break;
    case INTINT:
        st=cmd.fp2(atoi(cmd.args[1]),atoi(cmd.args[2]));
        break;
    default:
        break;
    }
    if(st==SUCCESS){
        strcat(buf2,"] success\n");
    }else{
        strcat(buf2,"] failed\n");
    }
    strcat(buf2,buf1);

    rio_writen(connfd,buf2,strlen(buf2));
}

static ssize_t rio_writen(int fd,void* usrbuf,size_t n){
    size_t nleft=n;
    ssize_t nwritten;
    char* bufp=usrbuf;
    while(nleft>0){
        if((nwritten=write(fd,bufp,nleft))<0 ){
            if(errno==EINTR) nwritten=0;
            else return -1;
        }
        nleft-=nwritten;
        bufp+=nwritten;
    }
    return n;
}