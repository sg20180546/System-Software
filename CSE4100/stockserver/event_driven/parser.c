#include "parser.h"


STATUS find_cmd(char buf[],struct command* cmd,int* p){

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
        for(j=0;j<3;j++){
            if(!strncmp(command_list[i].name[j],buf,pos)){
                (*cmd).flag=command_list[i].flag;
                strcpy(cmd->name[0],command_list[i].name[0]);
                switch ((*cmd).flag)
                {
                case 0x0:
                    (*cmd).fp0=command_list[i].fp0;
                    break;
                case 0x1:
                    (*cmd).fp1=command_list[i].fp1;
                    break;
                case 0x2:
                    (*cmd).fp2=command_list[i].fp2;
                    break;
                default:

                    break;
                }
            }
        }
    }

    if(cmd->name[0]==NULL){
        printf("here?");
        return ERROR;
    } 
    *p=pos; 
    
    return SUCCESS;
}

STATUS find_args(char* buf,struct command* cmd){
    printf("%s",buf);
    STATUS st=SUCCESS;
    unsigned int pos=0;
    switch (cmd->flag)
    {
    case (NOARGS||CHARP):
        while(whitespace(buf[pos])) pos++;
        if(buf[pos]!=ENTER) return ERROR;
        break;
    case INTINT:
        while(whitespace(buf[pos])) pos++;
        buf=&buf[pos]; pos=0;
        while(!whitespace(buf[pos])) pos++;
        strncpy(cmd->args[1],buf,pos);


        while(whitespace(buf[pos])) pos++;
        buf=&buf[pos]; pos=0;
        while(!whitespace(buf[pos]) ) pos++;
        strncpy(cmd->args[2],buf,pos);
        
        while(whitespace(buf[pos])) pos++;
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