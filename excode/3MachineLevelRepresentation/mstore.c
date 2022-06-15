long mult2(long ,long);
void mulstore(long x,long y ,long * dest){
    long t= mult2(x,y);
    *dest=t;
}


// Disassembly of section .text:

// 0000000000000000 <mulstore>:
//    0:   f3 0f 1e fa             endbr64 
//    4:   53                      push   %rbx
//    5:   48 89 d3                mov    %rdx,%rbx
//    8:   e8 00 00 00 00          callq  d <mulstore+0xd>
//    d:   48 89 03                mov    %rax,(%rbx)
//   10:   5b                      pop    %rbx
//   11:   c3                      retq  