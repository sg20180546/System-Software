long shift_left4_rightn(long x, long n){
    x<<=4;
    x>>=n;
    return x;
}

/*
x in %rdi, n in %rsi
movq %rdi, %rax
salq   $4,  %rax
// n is long , but the least significat byte is required to specify shift
movl %esi, %ecx
sarq  %cl, %rax
ret
*/