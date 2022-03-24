#include "../csapp.h"

int main(int argc,char** argv){

    int fd=open(argv[1],O_RDONLY);
    // assert(fd>=0);
    int rc=fork();
    if(rc==0){
        rc=lseek(fd,10,SEEK_SET);
        printf("child: offset:%d\n",rc);
    }else if(rc>0){
        (void) wait(NULL);
        printf("parent: offset %d\n",(int)lseek(fd,0,SEEK_CUR));
    }
    exit(0);
}