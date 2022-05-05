#include "stockfile_handle.h"

void read_stockfile(){
    char id[MAXLINE],price[MAXLINE],count[MAXLINE];
    int ID,COUNT,PRICE;
    fp=fopen(STOCK_FILE_PATH,"r+");
    if(fp==NULL) error_exit("NO STOCK FILE\n");
    while(!feof(fp)){
        fscanf(fp,"%s %s %s\n",id,count,price);
        ID=atoi(id);
        COUNT=atoi(count);
        PRICE=atoi(price);
        _root=insert(_root,ID,COUNT,PRICE);
    }
    
}

void fsync_stockfile(){
    t0=time(0);
    char buf[MAXLINE]="";
    int fd=open(STOCK_FILE_PATH,O_WRONLY);
    print_to_buf(_root,buf);
    write(fd,buf,strlen(buf)-1);
    fsync(fd);
    close(fd);
}

int time_check(){
    t1=time(0);
    double diff=difftime(t1,t0);
    if(diff>MAX_FSYNC_TIME){
        t0=time(0);
        return 1;
    }
    return -1;
}