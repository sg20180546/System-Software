#include "network.h"


static void execute(struct command cmd);
static void add_client(int connfd);

static void sendData(void *data, uint32_t datalen,int fd)
{
    uint32_t len = htonl(datalen);
    // send(fd, &len, sizeof(len), 0);
    send(fd, data, datalen, MSG_DONTWAIT);
}

static void execute(struct command cmd){

    if(cmd.name==NULL) return;

    char res[MAXLINE]="[";
    STATUS st;
    strcat(res,cmd.name);
    strcpy(cmd.result,"");
    st=cmd.fp(&cmd);
    


    if(st==SUCCESS){
        strcat(res,"]\033[0;32msuccess\n\033[0m");
    }else if(ERROR){
        strcat(res,"] failed\n");
    }else if(NOTENOUGHERR){
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
    // printf(" see pool\n");
    _pool.ready_set=_pool.read_set;
    _pool.nready=select(_pool.maxfd+1,&_pool.ready_set,NULL,NULL,NULL);

    if(FD_ISSET(listenfd,&_pool.ready_set)){
        clientlen=sizeof(struct sockaddr_storage);
        int connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen);
        // connfd=accept4()
        int flag=fcntl(connfd,F_GETFL,0);
        fcntl(connfd,F_SETFL,flag|O_NONBLOCK);
        add_client(connfd);
        char clienthostname[MAXLINE],clientport[MAXLINE];
        getnameinfo((SA*)&clientaddr,sizeof(clientaddr),clienthostname,MAXLINE,clientport,MAXLINE,0);
        printf("Connected to (%s:%s)\n",clienthostname,clientport);
    }
    // printf("exit see poool\n");
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
        int p=0;
        if((cmd.connfd>0)&&(FD_ISSET(cmd.connfd,&_pool.ready_set))){
            _pool.nready--;
            if((rc=rio_readlineb(&rio,buf,MAXLINE))!=0){
                printf("Server received %ld bytes\n",rc);
                st=parser(buf,rc,&cmd);

                if(st==NOCMD){
                    Rio_writen(cmd.connfd,"no such command\n",MAXLINE);
                    // continue;
                }else if(st==INVARG){
                    Rio_writen(cmd.connfd,"invalid argument\n",MAXLINE);
                    // continue;
                }else if(st==NL){
                    Rio_writen(cmd.connfd,"please type command\n",MAXLINE);
                    // continue;
                }else execute(cmd);
                // int prev=cmd.connfd;
                // cmd.connfd=accept(cmd.connfd,(struct sockaddr*)&clientaddr,&clientlen);
                // close(prev);
                // add_client(cmd.connfd);
                // close(prev);
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






