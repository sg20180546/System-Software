#include "common_test.h"
#include "../stockfile_handle.h"
#include "../binary_tree.h"
int node_count=0;
int main(int argc,char**argv){
    int fd;
    start=clock();
    StartPhase("Test transform in-memory binary tree to in-memory buffer");
    {
        int id,count,price;
        for(;;){

        }
    }

    StartPhase("Test fsync binary tree to on-disk file");
    {

    }
    StartPhase("Test Does contents of on-disk file equals to that of in-memory buffer");
    {   
        char on_disk[MAXLINE];
        fd=open(STOCK_FILE_PATH,O_WRONLY);
        read(STOCK_FILE_PATH,on_disk,MAXLINE);

        char in_memory[MAXLINE];
        print_to_buf(_root,in_memory);

        int result=strcmp(in_memory,on_disk);
        CheckConditon(result==0);
    }

    StartPhase("Test time_check function using when periodically fsync file");
    {

    }


    end=clock();
    time_taken= (end-start)/CLOCKS_PER_SEC;
    fprintf(stderr,"%s took %f seconds to execute\nPASS\n",argv[0]+2,time_taken);
    return 0;
}