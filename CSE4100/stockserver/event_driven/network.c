#include "network.h"


static void execute(struct command cmd);
static void add_client(int connfd);



static void execute(struct command cmd){

    if(cmd.name[0]==NULL) return;

    char res[MAXLINE]="[";
    STATUS st;
    strcat(res,cmd.name[0]);
    strcpy(cmd.result,"");
    st=cmd.fp(&cmd);
    


    if(st==SUCCESS){
        strcat(res,"]\033[0;32msuccess\n\033[0m");
    }else{
        strcat(res,"] failed\n");
    }
    strcat(res,cmd.result);

    rio_writen(cmd.connfd,res,strlen(res));
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


void see_pool(void){

    _pool.ready_set=_pool.read_set;
    _pool.nready=select(_pool.maxfd+1,&_pool.ready_set,NULL,NULL,NULL);

    if(FD_ISSET(listenfd,&_pool.ready_set)){
        clientlen=sizeof(struct sockaddr_storage);
        int connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen);
        add_client(connfd);
    }

}

void write_pool(void){

    int i,connfd;
    ssize_t rc;
    rio_t rio;
    STATUS st;
    for(i=0;(i<=_pool.maxi)&&(_pool.nready>0);i++){
        struct command cmd;
        char buf[MAXLINE];
        cmd.connfd=_pool.clientfd[i];
        cmd.poolidx=i;
        rio=_pool.clientrio[i];
        int p=0;
        if((cmd.connfd>0)&&(FD_ISSET(cmd.connfd,&_pool.ready_set))){
            _pool.nready--;
            if((rc=rio_readlineb(&rio,buf,MAXLINE))!=0){
    
                st=parser(buf,rc,&cmd);

                if(st==NOCMD){
                    rio_writen(cmd.connfd,"no such command\n",16);
                    continue;
                }else if(st==INVARG){
                    rio_writen(cmd.connfd,"invalid argument\n",20);
                    continue;
                }else if(st==NL){
                    rio_writen(cmd.connfd,"please type command\n",20);
                    continue;
                }

                execute(cmd);
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






