#include "network.h"


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
    struct command cmd;
    cmd.connfd=connfd;
    rio_readinitb(&rp,connfd);
    // thread_safe_printf("connfd :%d\n",connfd);
    while(1){ 
        if((rc=rio_readlineb(&rp,thread_safe_local_buf,MAXLINE))!=0){
            thread_safe_printf("Server received %ld bytes\n",rc);
            st=parser(thread_safe_local_buf,rc,&cmd);

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
                execute(cmd);
                break;
            }
            free_args(&cmd);
        }else{ 
            socket_close(connfd);
            break;
        }
    }
}