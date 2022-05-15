// 1
int my_func(int x, int y){
    if(x<y)
        if(2*x<y) return y-2*x;
        else return y-x;
    else return y+x;
}
/*
x in %rdi y in %rsi
movq %rsi, %rax
cmpq %rdi, %rsi
jl   .L4
subq %rdi, %rax
cmpq %rdi, %rax
jl   .L5
ret
.L4:
addq %rdi, %rax
ret
.L5
subq %rdi, %rax
ret
*/

// 2
#define MAX 10
int func(int a){
    unsigned short i;
    int result=a;
    for(i=0;i<MAX;i++){
        result+=a*(i+1);
    }
    return result;
}

//func(1)==3628800
// 1+2+3+4+5+6+7+8+9+1=
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


//3
// %rsp 0x800070 %rbp 0x8000F0
proc:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    addq $-24, %rsp
    leaq -8(%rbp), %rax
    pushq %rax
    leaq -16(%rbp), %rax
    pushq %rax
    leaq -24(%rbp), %rax
    pushq %rax
    pushq $3
    (t)
    call subproc
    movq %rbp, %rsp
    popq %rbp
    ret
