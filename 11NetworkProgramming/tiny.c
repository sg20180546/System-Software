#include "../csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char* uri,char* filename,char* cgiargs);
void serve_static(int fd,char* filename,int filesize);
void get_filetype(char* filename,char* filetype);
void serve_dynamic(int fd,char*filename,char* cgiargs);
void clienterror(int fd,char*cause,char*errnum,char*shortmsg,char*longmsg);

int main(int argc,char** argv){
    int listenfd,connfd;
    char hostname[MAXLINE],port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if(argc!= 2){
        fprintf(stderr,"usage : %s <port>\n",argv[0]);
        exit(1);
        }

    listenfd=Open_listenfd(argv[1]);
    while(1){
        clientlen=sizeof(clientaddr);
        connfd=Accept(listenfd,(SA*)&clientaddr,&clientlen);
        Getnameinfo((SA*)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
        printf("Accepted connection from (%s %s)\n",hostname,port);
        doit(connfd);
        Close(connfd);
    }
}