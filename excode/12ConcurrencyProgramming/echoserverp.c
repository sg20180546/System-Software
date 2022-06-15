#include "../csapp.h"

void ehco(int connfd);

void sigchild_handler(int sig){
    while(waitpid(-1,0,WNOHANG)>0);
    return;
}

int main(int argc, char** argv){
    int listenfd,connfd,port;
    socklen_t clientlen=sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    if(argc!=2){
        fprintf(stderr,"usage :%s <port>\n",argv[0]);
    }
    port=atoi(argv[1]);

    Signal(SIGCHLD,sigchild_handler);
    listenfd=Open_listenfd(port);
    while(1){
        connfd=Accept(listenfd,(SA*) &clientaddr,&clientlen);
        if(Fork()==0){
            Close(listenfd);
            echo(connfd);
            Close(connfd);
            exit(0);
        }
        Close(connfd);
    }
}