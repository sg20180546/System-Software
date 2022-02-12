#include "../csapp.h"
// #define DEF_MODE S_IRUSR|S_IWUSR|S_IXUSR
int main(){
    int fd1,fd2;
    fd1=Open("foo.text",O_RDONLY|O_CREAT,DEF_MODE);
    Close(fd1);
    fd2=Open("baz.txt",O_RDONLY|O_CREAT,DEF_MODE);
    printf("fd2=%d\n",fd1);
    exit(0);
}