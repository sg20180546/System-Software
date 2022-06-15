#include <stdio.h>
#include <stdlib.h>
struct ab
{
    int n,a;
};

int main(){
    struct ab* a,*b;
    a=malloc(sizeof(struct ab));
    printf("sizeof struct: %ld\n",sizeof(struct ab));
    printf("header? : %p\n",(a-1) );
    printf("struct ref : %p\n",a);
    printf("struct->n ref : %p\n",&(a->n));
    b=malloc(sizeof(struct ab));
    printf("struct b ref: %p\n",b);
}