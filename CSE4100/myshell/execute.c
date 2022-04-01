#include "execute.h"


void execute_commands(struct command* cur_cmd){
    pid_t pid;
    // flush stdin
    if(cur_cmd==NULL){
        return;
    }
    if((pid=fork())==0){
        dup2(fds[0],STDIN_FILENO);

        if(cur_cmd->redirectto){
            dup2(fds[1],STDOUT_FILENO);
            close(fds[0]); close(fds[0]);
        }
        // pd("in exe");
        // printf("argc %d ",cur_cmd->argc);
        a(cur_cmd->arguments[cur_cmd->argc]==NULL);
        char path[20]="/bin/";
        if(cur_cmd->f==STATIC) strcat(path,cur_cmd->static_cmd->name);
        if(cur_cmd->f==CUSTOM) strcat(BUILTIN_PATH,cur_cmd->cstm_cmd->name);
        // pd(path);
        
        // printf("cur %s len %ld\n",cur_cmd->arguments[0] ,strlen(cur_cmd->arguments[1]));
        // printf("cur %s\n",cur_cmd->arguments[2]);
        int debug=execve(path,cur_cmd->arguments,NULL);
        a(debug>=0);
    }
    waitpid(pid,NULL,0);
    execute_commands(cur_cmd->redirectto);

    close(fds[0]); close(fds[1]);
    // if background return; and set signal hander
}

