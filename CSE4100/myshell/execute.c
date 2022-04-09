#include "execute.h"

static void switch_case_cmd(struct command* cur_cmd);

void execute_commands(struct command* cur_cmd){
    
    if(!cur_cmd){
        exit(0);
    }
    pid_t pid;
    int i=0;
    pipe(fds[i]);

    while(cur_cmd){
        pipe(fds[i+1]);
        if((pid=fork())==0){
            if(cur_cmd->redirectfrom){
                dup2(fds[i][READ_END],STDIN_FILENO);
            }
            if(cur_cmd->redirectto){
                dup2(fds[i+1][WRITE_END],STDOUT_FILENO);
            }
            close(fds[i][READ_END]); close(fds[i][WRITE_END]);
            close(fds[i+1][READ_END]); close(fds[i+1][WRITE_END]);
            switch_case_cmd(cur_cmd);
        }else{
            close(fds[i][READ_END]);
            close(fds[i+1][WRITE_END]);
            cur_cmd=cur_cmd->redirectto;
            pid_list[i]=pid;
            i++;
        }
    }
}



void execute_function_command(struct command* func_cmd){
    if(func_cmd->f!=FUNCTION){
        fprintf(stderr,"FUNCTION COMMAND ERROR\n");
    }
    func_cmd->builtin->fp(func_cmd->arguments);
    pid_t pp=getppid();
    SEND_CONTINUE(pp);
}

static void switch_case_cmd(struct command* cur_cmd){
    if(cur_cmd->f==FUNCTION){
        execute_function_command(cur_cmd);
    }else if(cur_cmd->f==ABSOLUTE){
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
}
