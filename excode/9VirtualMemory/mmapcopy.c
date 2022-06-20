#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include "../csapp.h"
// mode_t mode;
// int main(int argc,char** argv){
//     int fd=Open(argv[2],O_CREAT|O_WRONLY,mode);
//     mmap(NULL,30,PROT_READ,MAP_ANON,fd,0);
// }

void mmapcopy(int fd,int size)
{
    char* bufp;
    bufp=Mmap(NULL,size,PROT_READ,MAP_PRIVATE,fd,0);
    Write(1,bufp,size);
    return;
}

int main(int argc, char**argv){
    struct stat stat;
    int fd;
    if(argc!=2){
        printf("usage: %s <filename>\n",argv[0]);
        fd=Open(argv[1],O_RDONLY,0);
        fstat(fd,&stat);
        mmapcopy(fd,stat.st_size);
        exit(0);
    }
}