#include "execute.h"


void execute_commands(command* cur_cmd){
    int fds[2];
    pid_t pid;
    pipe(fds);
    // flush stdin

    
    if((pid=fork())==0){
        dup2(fds[0],STDIN_FILENO);
        if(cur_cmd->redirectto){
            dup2(fds[1],STDOUT_FILENO);
            close(fds[0]); close(fds[0]);
        } 
        
        char path[10]="/bin/";
        if(cur_cmd->f==STATIC) strcat(path,cur_cmd->static_cmd->name);

        execve(path,cur_cmd->arguments,NULL);
    }
    
    if(cur_cmd->redirectto) execute_commands(cur_cmd->redirectto);

    close(fds[0]); close(fds[1]);
    // if background return; and set signal hander
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
}

