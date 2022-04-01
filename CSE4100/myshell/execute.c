#include "execute.h"


void execute_commands(struct command* cur_cmd){
    pid_t pid;
    // flush stdin
    if(cur_cmd==NULL){
        return;
    }
    pd("in exe 1");
    if((pid=fork())==0){
        dup2(fds[0],STDIN_FILENO);
        a(cur_cmd->redirectto==NULL);
        if(cur_cmd->redirectto){
            dup2(fds[1],STDOUT_FILENO);
            close(fds[0]); close(fds[0]);
        } 
        pd("in exe");
        char path[20]="/bin/";
        if(cur_cmd->f==STATIC) strcat(path,cur_cmd->static_cmd->name);
        if(cur_cmd->f==CUSTOM) strcat(BUILTIN_PATH,cur_cmd->cstm_cmd->name);
        assert(path=="/bin/ls");
        int debug=execve(path,cur_cmd->arguments,NULL);
        a(debug>=0);
    }
    waitpid(pid,NULL,0);
    execute_commands(cur_cmd->redirectto);

    close(fds[0]); close(fds[1]);
    // if background return; and set signal hander
}

