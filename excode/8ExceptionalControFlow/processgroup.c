#include <unistd.h>
#include <stdio.h>

int main(){
    pid_t pid1=getpid();
    pid_t pidg1=getpgrp();


    printf("cur pid : %d pgroup: %d\n",pid1,pidg1);
    int a;
    setpgid(0,0);
    printf("cur pid : %d pgroup: %d\n",pid1,pidg1);
    while(1){
        scanf("%d",&a);
        if(a==1) break;
    }
    return 0;
}