#include "execute.h"


void execute_commands(command* cmd_list,unsigned short pipen){
    int fds[2];
    unsigned short i=0;
    pipe(fds);
    do{
        execute_command(cmd_list[i++],&fds[0],&fds[1]);
    }while(i<pipen);
    close(fds[0]); close(fds[1]);
}


static void execute_command(command* cmd,int* readfd,int* writefd){
    if(fork()==0){
        char buf[MAXLINE];
        // read(readfd,buf,)
        dup2(*readfd,STDIN_FILENO); dup2(*writefd,STDOUT_FILENO);
        if(cmd->f==STATIC){
            
        }else{

        }

    }


    free_all_command();
}


static void free_all_command(){

}