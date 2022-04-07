#include "execute.h"


void execute_commands(struct command* cur_cmd){
    pid_t pid;
    if(!cur_cmd){
        return;
    }
    // fprintf(stderr,"%s",cur_cmd->builtin->name);
    if((pid=fork())==0){
        if(cur_cmd->redirectfrom){
            dup2(fds[READ_END],STDIN_FILENO);
        }
        
        if(cur_cmd->redirectto){
            dup2(fds[WRITE_END],STDOUT_FILENO);
        } 
    
        close(fds[READ_END]); close(fds[WRITE_END]);

        
        if(cur_cmd->f==FUNCTION){
            execute_function_command(cur_cmd);
        }else if(cur_cmd->f==ABSOLUTE){
            // a(strcmp(cur_cmd->arguments[1],"test.txt")==0);
            char path[20]="/bin/";
            strcat(path,cur_cmd->builtin->name);
            if(execv(path,cur_cmd->arguments)<0){
                fprintf(stderr,"Executing falied\n");
            }
        }else if(cur_cmd->f==RELATIVE){
    
            if(execv(cur_cmd->builtin->name,cur_cmd->arguments)<0){
                fprintf(stderr,"MYSHELL: %s : No such executable file",cur_cmd->builtin->name);
            }
            
        }
        
    

        exit(0);
    }else{
        waitpid(pid,NULL,0);
        // pause();
        if(!cur_cmd->redirectto){
            close(fds[READ_END]); close(fds[WRITE_END]);
        }
        // fprintf(stderr,"waiting %s\n",cur_cmd->builtin->name);
        execute_commands(cur_cmd->redirectto);

    }
}

void execute_function_command(struct command* func_cmd){
    if(func_cmd->f!=FUNCTION){
        fprintf(stderr,"FUNCTION COMMAND ERROR\n");
    }
    func_cmd->builtin->fp(func_cmd->arguments);
}