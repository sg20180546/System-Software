#include "execute.h"


void execute_commands(command* cur_cmd){
    int fds[2];
    pid_t pid;
    pipe(fds);
    // flush stdin
    if(cur_cmd==NULL) return;
    
    if((pid=fork())==0){
        dup2(fds[0],STDIN_FILENO);
        if(cur_cmd->redirectto){
            dup2(fds[1],STDOUT_FILENO);
            close(fds[0]); close(fds[0]);
        } 
        
        char path[20]="/bin/";
        if(cur_cmd->f==STATIC) strcat(path,cur_cmd->static_cmd->name);
        if(cur_cmd->f==CUSTOM) strcat(BUILTIN_PATH,cur_cmd->cstm_cmd->name);
        if(execve(path,cur_cmd->arguments,NULL)<0){
            exit(33);
            // error handling
        }
    }
    
    execute_commands(cur_cmd->redirectto);

    close(fds[0]); close(fds[1]);
    // if background return; and set signal hander
}

