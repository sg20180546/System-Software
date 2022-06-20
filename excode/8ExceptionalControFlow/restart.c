#include "csapp.h"

sigjmp_buf buf;

void handler(int sig){
    siglongjmp(buf,1);
}


int main(){

    if(sigsetjmp(buf,1)){
        sio_puts("restarting\n");
    }else{
        Signal(SIGINT,handler);
        sio_puts("starting\n");
    }
    while(1){
        sleep(1);
        sio_puts("processing...\n");
    }
    exit(0);//never reach here
}