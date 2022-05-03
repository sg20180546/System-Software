#include "impl.h"

static void parser();

STATUS show(int connfd){
    char buf[MAXLINE]="";
    print_to_buf(root,buf);
    write(connfd,buf,strlen(buf));
    return SUCCESS;
}

STATUS sell(int connfd,int id,int count){
    // if count -?
    modify(id,count);
}

void event_driven(){

}