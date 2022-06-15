#include <unistd.h>
#include <stdio.h>

signed int wakeup(unsigned int secs){
    unsigned int rc=sleep(secs);
    printf("Woke up at %d secs",rc);
    return rc;
}

int main(){
    wakeup(5);
}