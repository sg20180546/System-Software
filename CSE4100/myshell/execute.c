#include "execute.h"


void execute_commands(command** cmd_list,unsigned short pipen){
    int fds[2];
    unsigned short i=0;
    pipe(fds);
    do{
        execute_command(cmd_list[i],&fds[0],&fds[1]);
    }while(i<pipen);
}


static void execute_command(command* cmd,int* readfd,int* writefd){
    if(fork()==0){
        dup2(*readfd,STDIN_FILENO);
        if(cmd->f==STATIC){

        }else{

        }

    }
}
