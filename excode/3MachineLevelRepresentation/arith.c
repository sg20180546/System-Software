long arith(long x, long y,long z)
{
    long t1=x^y;
    long t2=z*48;
    long t3=t1&0x0F0F0F0F;
    long t4= t2-t3;
    return t4;
}

/*
x in %rdi, y in %rsi, z in %rdx
xorq %rsi, %rdi // t1=x^y
leaq (%rdx,%rdx,2), %rax // %rax= 3*z
salq $4, %rax // t2=3z*16
andl $252645135, %edi
subq %rdi, %rax
*/