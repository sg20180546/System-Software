#include "parser.h"


STATUS find_cmd(char buf[],struct command* cmd,int* p){
    cmd->args=calloc(1,sizeof(char*)*4);
    int i,j;
    size_t n;
    unsigned int pos=0;
    if(buf[pos]==ENTER){

        return ERROR;
    }
    while(! whitespace( buf[pos] ) ){
        if(buf[pos]==ENTER) break;
        pos++; 
    }

    for(i=0;i<COMMAND_N ;i++){

        if(!strncmp(command_list[i].name,buf,pos)){
            (*cmd).flag=command_list[i].flag;
            cmd->args[0]=calloc(1,10);
            cmd->argc=1;
            strcpy(cmd->name,command_list[i].name);
            (*cmd).fp=command_list[i].fp;    
        }
    }

    if(cmd->name==NULL){
        return ERROR;
    } 
    *p=pos; 
    
    return SUCCESS;
}

STATUS find_args(char* buf,struct command* cmd){
    STATUS st=SUCCESS;
    unsigned int pos=0;
    
    switch (cmd->flag)
    {
    case NOARGS:
        while(whitespace(buf[pos])) pos++;
        if(buf[pos]!=ENTER) return ERROR;
        break;
    case INTINT:
        while(whitespace(buf[pos])) pos++;
        buf=&buf[pos]; pos=0;
        while(!whitespace(buf[pos])) pos++;
        cmd->args[1]=calloc(1,sizeof(char)*(pos+1));
        strncpy(cmd->args[1],buf,pos);
        while(whitespace(buf[pos])) pos++;
        buf=&buf[pos]; pos=0;
        while(!whitespace(buf[pos]) ){
            if(buf[pos]==ENTER) break;
            pos++;
        }
        // printf("second arg pos : %d\n",pos);
        cmd->args[2]=calloc(1,sizeof(char)*(pos+1));
        strncpy(cmd->args[2],buf,pos);
        // printf("cpyargs: %s, buf : %s",cmd->args[2],buf);
        while(whitespace(buf[pos])) pos++;
        cmd->argc=3;
        if(buf[pos]!=ENTER) return ERROR;

        break;
    default:
        break;
    }
    return SUCCESS;

}

STATUS parser(char* buf,int rc,struct command* cmd){
    if(rc==1) return NL;
    int p;
    STATUS st;
    st=find_cmd(buf,cmd,&p);
    if(st==ERROR) return NOCMD;
    
    st=find_args((buf+p),cmd);

    if(st==ERROR) return INVARG;

    return SUCCESS;
}