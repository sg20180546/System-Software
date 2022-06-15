#include "network.h"


static void free_args(struct connection* cnct){
    int i;
    for(i=0;i<cnct->argc;i++){
        free(cnct->args[i]);
    }
    free(cnct->args);
}

static void execute(struct connection cmd){
    if(cmd.name==NULL) return;

    char res[MAXLINE]="\033[0m[";
    STATUS st;
    strcat(res,cmd.name);
    strcpy(cmd.result,"");
    st=cmd.fp(&cmd);

    if(st==SUCCESS) strcat(res,"]\033[0;32msuccess\n\033[0m");
    else if(st==ERROR) strcat(res,"] failed\n");
    else if(st==NOTENOUGHERR)  strcpy(res,"NOT ENOUGH LEFT STOCK\n");
    strcat(res,cmd.result);
    Rio_writen(cmd.connfd,res);
}


void service(int connfd,char* hostname,char* port){
    char thread_safe_local_buf[MAXLINE];
    rio_t rp;
    STATUS st;
    ssize_t rc;
    struct connection cnct;
    cnct.clienthostname=hostname;
    cnct.clientport=port;
    cnct.connfd=connfd;
    rio_readinitb(&rp,connfd);
    while(1){ 
        if((rc=rio_readlineb(&rp,thread_safe_local_buf,MAXLINE))!=0){
            thread_safe_printf("\033[0mServer received %ld bytes\n",rc);
            st=parser(thread_safe_local_buf,rc,&cnct);

            switch (st)
            {
            case NOCMD:
                Rio_writen(connfd,"No Such Command\n");
                break;
            case INVARG:
                Rio_writen(connfd,"Invalid Argument\n");
                break;
            case NL:
                Rio_writen(connfd,"Please Type Command\n");
                break;
            default:
                execute(cnct);
                break;
            }
            free_args(&cnct);
        }else{ 
            socket_close(connfd,hostname,port);
            break;
        }
    }
}