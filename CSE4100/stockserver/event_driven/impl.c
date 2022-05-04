#include "impl.h"


struct command command_list[]={
    {.flag=0x1,.name={"show","SHOW","Show"},.fp1=show},
    {.flag=0x2,.name={"sell","SELL","Sell"},.fp2=sell},
    {.flag=0x2,.name={"buy","BUY","Buy"},.fp2=buy},
    {.flag=0x2,.name={"exit","EXIT","Exit"},.fp2=exit_client}
};
STATUS show(char* buf){
    return print_to_buf(_root,buf);
}

STATUS sell(int id,int count){
    return modify(id,count);
}

STATUS buy(int id, int count){
    return modify(id,-count);
}

STATUS exit_client(int connfd,int idx){
    if(remove_client(connfd,idx)) return SUCCESS;
    return ERROR;
}


