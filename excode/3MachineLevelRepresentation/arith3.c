short arith3(short x,short y, short z){
    short p1=z|y;
    short p2=p1>>9;
    short p3=~p2;
    short p4=y-p3;
    return p4;
}


/*
x in %rdi , y in %rsi, z in %rdx
arith3:
    orq %rsi,%rdx // p1=z|y
    sarq $9, %rdx // p2=p1>>9
    notq %rdx // %rdx= p3=~p2
    movq %rdx, %bax // %rbx in p3
    subq %rsi, %rbx // 


arith3:
 orq %rsi, %rdx 
 sarq $9, %rdx 
 notq %rdx 
 movq %rdx, %rbx  
 subq %rsi, %rbx 
 ret
 */