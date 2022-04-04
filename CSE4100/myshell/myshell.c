#include "myshell.h"

// [1] 20878 background [1]+  Done
int main(){

    parent_pid=getpid();

    while(1){
        interpreter(cmdline);

    }

    return 0;
}

