#include <stdio.h>

void mulstore(long,long,long*);

int main(){
    long d;
    mulstore(d,3,&d);
    printf("2*3-->%ld\n",d);
    return 0;
}
long mult2(long a,long b){
    long s=a*b;
    return s;
}