#include "impl.h"



struct command command_list[]={
    {.flag=0x0,.name="show",.fp=show},
    {.flag=0x1,.name="sell",.fp=sell},
    {.flag=0x1,.name="buy",.fp=buy},
    {.flag=0x0,.name="exit",.fp=exit_client}
};
STATUS show(struct command* cmd){
    return print_to_buf(_root,cmd->result);
}

STATUS sell(struct command* cmd){
    int args1=atoi(cmd->args[1]);
    int args2=atoi(cmd->args[2]);
    return modify(args1,args2);
}

STATUS buy(struct command* cmd){
    // printf("arg1 %s arg2 %s\n",cmd->args[1],cmd->args[2]);
    int args1=atoi(cmd->args[1]);
    int args2=atoi(cmd->args[2]);
   
    return modify(args1,-args2);
}

STATUS exit_client(struct command* cmd){
    if(remove_client(cmd->connfd,cmd->poolidx)){
        return SUCCESS;
    }
    return ERROR;
}


