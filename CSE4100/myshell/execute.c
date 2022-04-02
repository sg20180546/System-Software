#include "execute.h"


void execute_commands(struct command* cur_cmd){
    pid_t pid;
    // flush stdin
    
    if(!cur_cmd){
        return;
    }

    if((pid=fork())==0){
        if(cur_cmd->redirectfrom!=NULL){
            dup2(fds[READ_END],STDIN_FILENO);
        } 
        if(cur_cmd->redirectto!=NULL){
            dup2(fds[WRITE_END],STDOUT_FILENO);
        } 
    
        close(fds[READ_END]); close(fds[WRITE_END]);

        char path[20]="/bin/";
        if(cur_cmd->f==STATIC) strcat(path,cur_cmd->static_cmd->name);
        if(cur_cmd->f==CUSTOM) strcat(BUILTIN_PATH,cur_cmd->cstm_cmd->name);
        

        if(execve(path,cur_cmd->arguments,NULL)<0){
            fprintf(stderr,"Executing falied\n");
            exit(33);
        }
        
    }else{
        if(!cur_cmd->redirectto){
            close(fds[READ_END]); close(fds[WRITE_END]);
        }

        waitpid(pid,NULL,0);

        execute_commands(cur_cmd->redirectto);
    }
}

