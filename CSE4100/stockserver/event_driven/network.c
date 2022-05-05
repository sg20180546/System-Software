#include "network.h"


static void execute(int connfd,struct command cmd);
static void add_client(int connfd);



static void execute(int connfd,struct command cmd){
    if(cmd.name[0]==NULL) return;
    char buf0[MAXLINE]="";
    char buf1[MAXLINE]="[";
    STATUS st;
    strcat(buf1,cmd.name[0]);

    switch (cmd.flag)
    {
    case NOARGS:
        st=cmd.fp0();
        break;
    case CHARP:
        st=cmd.fp1(buf0);
        break;
    case INTINT:
        st=cmd.fp2(atoi(cmd.args[1]),atoi(cmd.args[2]));
        break;
    default:
        break;
    }
    
    if(st==SUCCESS){
        strcat(buf1,"] success\n");
    }else{
        strcat(buf1,"] failed\n");
    }
    strcat(buf1,buf0);
    


    rio_writen(connfd,buf1,strlen(buf1));
}



void init_pool(int listenfd){
    int i;
    _pool.n=0;
    _pool.maxi=-1;
    for(i=0;i<FD_SETSIZE;i++) _pool.clientfd[i]=-1;
    _pool.maxfd=listenfd;
    FD_ZERO(&(_pool.read_set));
    FD_SET(listenfd,&(_pool.read_set));
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
    printf("see pool start ! \n");
    _pool.ready_set=_pool.read_set;
    _pool.nready=select(_pool.maxfd+1,&_pool.ready_set,NULL,NULL,NULL);
    // select bug
    printf("is selecting?\n");
    if(FD_ISSET(listenfd,&_pool.ready_set)){
        printf("in iseet\n");
        clientlen=sizeof(struct sockaddr_storage);
        connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen);
        add_client(connfd);
    }
    printf("see pool end ! \n");
}

void write_pool(void){
    printf("write pool start\n");
    int i,connfd;
    ssize_t rc;
    rio_t rio;
    STATUS st;
    for(i=0;(i<=_pool.maxi)&&(_pool.nready>0);i++){
        struct command cmd;
        char buf[MAXLINE];
        connfd=_pool.clientfd[i];
        rio=_pool.clientrio[i];
        int p=0;
        if((connfd>0)&&(FD_ISSET(connfd,&_pool.ready_set))){
            _pool.nready--;
            if((rc=rio_readlineb(&rio,buf,MAXLINE))!=0){
                printf("parsing... %ld buf: %s\n",rc,buf);
                st=parser(buf,rc,&cmd);
                // printf("read size : %ld buf:  %s",rc,buf);
                // if(rc==1) continue;
                // if(find_cmd(buf,&cmd,&p)==ERROR){
                //     rio_writen(connfd,"no such command\n",16);
                //     continue;
                // }

                // if(find_args((buf+p),&cmd)==ERROR){
                //    rio_writen(connfd,"invalid argument\n",20);
                //     continue;
                // }
                if(st==NOCMD){
                    rio_writen(connfd,"no such command\n",16);
                    continue;
                }else if(st==INVARG){
                    rio_writen(connfd,"invalid argument\n",20);
                    continue;
                }else if(st==NL){
                    continue;
                }
                // sprintf(cmd.args[0],"%d",i);
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
    // printf("%d %d\n",_pool.clientfd[idx],_pool.n);
    if(close(connfd)<0) return -1;
    FD_CLR(connfd,&_pool.read_set);
    _pool.clientfd[idx]=-1;
    _pool.n--;
    return 1;
}






