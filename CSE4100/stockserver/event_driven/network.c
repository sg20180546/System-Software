#include "network.h"


static void execute(struct command cmd);
static void add_client(int connfd);

static void free_args(struct command* cmd){
    int i;
    for(i=0;i<cmd->argc;i++){
        free(cmd->args[i]);
    }
    free(cmd->args);
}

static void execute(struct command cmd){

    if(cmd.name==NULL) return;

    char res[MAXLINE]="\033[0m[";
    STATUS st;
    strcat(res,cmd.name);
    strcpy(cmd.result,"");
    st=cmd.fp(&cmd);
    


    if(st==SUCCESS){
        strcat(res,"]\033[0;32msuccess\n\033[0m");
    }else if(st==ERROR){
        strcat(res,"] failed\n");
    }else if(st==NOTENOUGHERR){
        strcpy(res,"NOT ENOUGH LEFT STOCK\n");
    }
    strcat(res,cmd.result);
    Rio_writen(cmd.connfd,res,MAXLINE);
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
        int flag=fcntl(connfd,F_GETFL,0);
        fcntl(connfd,F_SETFL,flag|O_NONBLOCK);
        add_client(connfd);
        char clienthostname[MAXLINE],clientport[MAXLINE];
        getnameinfo((SA*)&clientaddr,sizeof(clientaddr),clienthostname,MAXLINE,clientport,MAXLINE,0);
        printf("Connected to (%s:%s)\n",clienthostname,clientport);
    }
}

void write_pool(void){
// printf("write poool\n");
    int i;
    ssize_t rc;
    rio_t rio;
    STATUS st;
    for(i=0;(i<=_pool.maxi)&&(_pool.nready>0);i++){
        struct command cmd;
        char buf[MAXLINE];
        cmd.connfd=_pool.clientfd[i];
        cmd.poolidx=i;
        rio=_pool.clientrio[i];
        if((cmd.connfd>0)&&(FD_ISSET(cmd.connfd,&_pool.ready_set))){
            _pool.nready--;
            if((rc=rio_readlineb(&rio,buf,MAXLINE))!=0){
                printf("Server received %ld bytes\n",rc);
                st=parser(buf,rc,&cmd);

                if(st==NOCMD){
                    Rio_writen(cmd.connfd,"No Such Command\n",MAXLINE);
                }else if(st==INVARG){
                    Rio_writen(cmd.connfd,"Invalid Argument\n",MAXLINE);
                }else if(st==NL){
                    Rio_writen(cmd.connfd,"Please Type Command\n",MAXLINE);
                }else execute(cmd);

                free_args(&cmd);
            }else{
                
                remove_client(cmd.connfd,i);
            }
        }
        

    }
    // printf("exit write poool\n");
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

    if(socket_close(connfd)<0) return -1;
    FD_CLR(connfd,&_pool.read_set);
    _pool.clientfd[idx]=-1;
    _pool.n--;
    return 1;
}






