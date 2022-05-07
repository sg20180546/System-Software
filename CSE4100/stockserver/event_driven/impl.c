#include "impl.h"

static void free_args(struct command* cmd){
    int i;
    for(i=0;i<cmd->argc;i++){
        free(cmd->args[i]);
    }
}

struct command command_list[]={
    {.flag=0x0,.name={"show","SHOW","Show"},.fp=show},
    {.flag=0x1,.name={"sell","SELL","Sell"},.fp=sell},
    {.flag=0x1,.name={"buy","BUY","Buy"},.fp=buy},
    {.flag=0x0,.name={"exit","EXIT","Exit"},.fp=exit_client}
};
STATUS show(struct command* cmd){
    free_args(cmd);
    return print_to_buf(_root,cmd->result);
}

STATUS sell(struct command* cmd){
    int args1=atoi(cmd->args[1]);
    int args2=atoi(cmd->args[2]);
    free_args(cmd);
    return modify(args1,args2);
}

STATUS buy(struct command* cmd){
    // printf("arg1 %s arg2 %s\n",cmd->args[1],cmd->args[2]);
    int args1=atoi(cmd->args[1]);
    int args2=atoi(cmd->args[2]);
    free_args(cmd);
    return modify(args1,-args2);
}

STATUS exit_client(struct command* cmd){
    free_args(cmd);
    if(remove_client(cmd->connfd,cmd->poolidx)){
        return SUCCESS;
    }
    return ERROR;
}


