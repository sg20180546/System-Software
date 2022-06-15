#include "stockfile_handle.h"

// Transfrom on-disk file to in-memory binary tree
void read_stockfile(){
    gettimeofday(&last_fsync_time,NULL);
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

// Transform in-memory binary tree to on-disk file
void fsync_stockfile(){
    gettimeofday(&last_fsync_time,NULL);
    char buf[MAXLINE]="";
    print_to_buf(_root,buf);
    
    int fd=open(STOCK_FILE_PATH,O_WRONLY);
    write(fd,buf,strlen(buf));
    fsync(fd);
    close(fd);
}

int time_check(){
    gettimeofday(&cur_time,NULL);
    double diff=cur_time.tv_sec+cur_time.tv_usec-last_fsync_time.tv_sec-last_fsync_time.tv_usec;
    if(diff>MAX_FSYNC_TIME){
        return 1;
    }
    return -1;
}