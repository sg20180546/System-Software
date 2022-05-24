#include <stdio.h>
#define MAX 10
int func(int a){
    unsigned short i=0;
    int result=a;
loop:
    if(!(i<MAX))
        goto done;
        result+=a*(i+1);
        i++;
        goto loop;
    done:
        return result;
}


int main(){
    printf("%d",func(1));
}