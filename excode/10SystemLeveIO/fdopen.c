#include "../csapp.h"

int main(){
    int fd1,fd2;
    fd1=open("foo.txt",O_RDONLY,0);
    close(fd1);
    fd2=open("baz.txt",O_RDONLY,0);
    printf("FD2= %d\n",fd2);
    exit(0);
}