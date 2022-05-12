#include "network.h"


static void execute(struct command cmd);
static void add_client(int connfd);

static void sendData(void *data, uint32_t datalen,int fd)
{
    uint32_t len = htonl(datalen);
    send(fd, &len, sizeof(len), 0);
    send(fd, data, datalen, 0);
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
    // printf("res %s",res);
    // char test[MAXLINE]="this\nis\nthe\ntest";
    strcat(res,"\r\n");
        // printf("strlen %ld\n",strlen(res));
    // pid_t pid=fork();

    // if(pid==0){
        sendData(res,strlen(res),cmd.connfd);

    //     close(cmd.connfd);
    //     exit(0);
    // }
    // else waitpid(pid,NULL,0);
    // Rio_writen(cmd.connfd,res,strlen(res));
    // send(cmd.connfd,res,strlen(res),MSG_NOSIGNAL);
    int flag=0;
    // https://stackoverflow.com/questions/26154074/c-socket-add-header-before-data-stream
    // https://stackoverflow.com/questions/45747973/in-c-how-to-indicate-eof-after-writing-data-without-close-shutdown-a-socket
    // flush socket descriptor
    // https://stackoverflow.com/questions/855544/is-there-a-way-to-flush-a-posix-socket
    // setsockopt(cmd.connfd,IPPROTO_TCP,TCP_NODELAY,(char*)&flag,sizeof(int));
    // shutdown(cmd.connfd,SHUT_RDWR);
    // Rio_writen(cmd.connfd,EOF,1);
    // fcntl(cmd.connfd);
    // close(cmd.connfd);
    // ioctl(cmd.connfd,0);
    // fsync(cmd.connfd);
    // fflus
    // send(cmd.connfd,test,strlen(test),0);
    // write(cmd.connfd,res,strlen(res));
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
                    Rio_writen(cmd.connfd,"no such command\n",16);
                    // continue;
                }else if(st==INVARG){
                    Rio_writen(cmd.connfd,"invalid argument\n",20);
                    // continue;
                }else if(st==NL){
                    Rio_writen(cmd.connfd,"please type command\n",20);
                    // continue;
                }else execute(cmd);
                int prev=cmd.connfd;
                cmd.connfd=accept(cmd.connfd,(struct sockaddr*)&clientaddr,&clientlen);
                // close(prev);
                add_client(cmd.connfd);
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






