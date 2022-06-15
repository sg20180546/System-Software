#include "parser.h"


STATUS find_cmd(char buf[],struct connection * cnct,int* p){
    cnct->args=calloc(1,sizeof(char*)*4);
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

        if(!strncmp(connection_frame[i].name,buf,pos)){
            (*cnct).flag=connection_frame[i].flag;
            cnct->args[0]=calloc(1,10);
            cnct->argc=1;
            strcpy(cnct->name,connection_frame[i].name);
            (*cnct).fp=connection_frame[i].fp;    
        }
    }

    if(cnct->args[0]==NULL){
        return ERROR;
    } 
    *p=pos; 
    
    return SUCCESS;
}

STATUS find_args(char* buf,struct connection * cnct){
    STATUS st=SUCCESS;
    unsigned int pos=0;
    
    switch (cnct->flag)
    {
    case NOARGS:
        while(whitespace(buf[pos])) pos++;
        if(buf[pos]!=ENTER) return ERROR;
        break;
    case INTINT:
        while(whitespace(buf[pos])) pos++;
        buf=&buf[pos]; pos=0;
        while(!whitespace(buf[pos])) pos++;
        cnct->args[1]=calloc(1,sizeof(char)*(pos+1));
        strncpy(cnct->args[1],buf,pos);
        while(whitespace(buf[pos])) pos++;
        buf=&buf[pos]; pos=0;
        while(!whitespace(buf[pos]) ){
            if(buf[pos]==ENTER) break;
            pos++;
        }
        // printf("second arg pos : %d\n",pos);
        cnct->args[2]=calloc(1,sizeof(char)*(pos+1));
        strncpy(cnct->args[2],buf,pos);
        // printf("cpyargs: %s, buf : %s",cnct->args[2],buf);
        while(whitespace(buf[pos])) pos++;
        cnct->argc=3;
        if(buf[pos]!=ENTER) return ERROR;

        break;
    default:
        break;
    }
    return SUCCESS;

}

STATUS parser(char* buf,int rc,struct connection * cnct){
    if(rc==1) return NL;
    int p;
    STATUS st;
    st=find_cmd(buf,cnct,&p);
    if(st==ERROR) return NOCMD;
    
    st=find_args((buf+p),cnct);

    if(st==ERROR) return INVARG;

    return SUCCESS;
}