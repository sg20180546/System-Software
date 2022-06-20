
#include "../csapp.h"

void handler1(int sig){
    int olderrrno=errno;
    if((waitpid(-1,NULL,0))<0) sio_error("waitpid error");
    sio_puts("Handler reaped child\n");
    sleep(1);
    errno=olderrrno;
}
void handler2(int sig){
    int olderrno=errno;
    while(waitpid(-1,NULL,0)>0){
        sio_puts("handler reaped child\n");
    }
    if(errno!=ECHILD) sio_error("wait pid error");
    Sleep(1);
    errno=olderrno;
}

int main(){
    int i,n;
    char buf[MAXBUF];
    if(signal(SIGCHLD,handler2)==SIG_ERR){
        unix_error("signal error");
    }
    for(i=0;i<3;i++){
        if(Fork()==0){
            printf("Hello from child %d\n",(int)getpid());
            exit(0);
        }
    }
    if( (n=read(STDIN_FILENO,buf,sizeof(buf)))<0 ){
        unix_error("read");
    }
    printf("parent processing input\n");
    while(1);
    exit(0);
}