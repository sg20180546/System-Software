#include "stockfile_handle.h"

void read_stockfile(){
    char id[MAXLINE],price[MAXLINE],count[MAXLINE];
    int ID,COUNT,PRICE;
    fp=fopen(STOCK_FILE_PATH,"r");
    if(fp==NULL) error_exit("NO STOCK FILE\n");
    while(!feof(fp)){
        fscanf(fp,"%s %s %s\n",id,count,price);
        ID=atoi(id);
        COUNT=atoi(count);
        PRICE=atoi(price);
        root=insert(root,ID,COUNT,PRICE);
    }
}