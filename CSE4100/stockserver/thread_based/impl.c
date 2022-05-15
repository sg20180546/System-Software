#include "impl.h"

struct command command_list[]={
    {.flag=0x0,.name="show",.fp=show},
    {.flag=0x1,.name="sell",.fp=sell},
    {.flag=0x1,.name="buy",.fp=buy},
    {.flag=0x0,.name="exit",.fp=exit_client}
};


STATUS show(struct command* cmd){
    STATUS st;
    
    while(writer_n); // if there is writer, reader queue stop increasing

    sem_wait(&reader_n_mutex);
    reader_n++;
    sem_post(&reader_n_mutex);

    st=print_to_buf(_root,cmd->result);

    sem_wait(&reader_n_mutex);
    reader_n--;
    sem_post(&reader_n_mutex);
    
    return st;
}

STATUS sell(struct command* cmd){
    STATUS st;
    int args1=atoi(cmd->args[1]);
    int args2=atoi(cmd->args[2]);
    

    sem_wait(&writer_n_mutex);
    writer_n++;
    sem_post(&writer_n_mutex);

    while(reader_n); // if there is reader, stop

    st=modify(args1,args2);
    
    sem_wait(&writer_n_mutex);
    writer_n--;
    sem_post(&writer_n_mutex);

    return st;
}

STATUS buy(struct command* cmd){
    STATUS st;
    int args1=atoi(cmd->args[1]);
    int args2=atoi(cmd->args[2]);
    


    sem_wait(&writer_n_mutex);
    writer_n++;
    sem_post(&writer_n_mutex);

    while(reader_n);

    st=modify(args1,-args2);

    sem_wait(&writer_n_mutex);
    writer_n--;
    sem_post(&writer_n_mutex);

    return st;
}

STATUS exit_client(struct command* cmd){
    
    if(socket_close(cmd->connfd)){
        return SUCCESS;
    }
    return ERROR;
}