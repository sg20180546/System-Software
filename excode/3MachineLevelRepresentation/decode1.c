/* 
    xp in %rdi, yp in %rsi, zp in %rdx
*/
void decode1(long *xp,long *yp,long *zp){
    int x=*xp; // movq (%rdi), %r8
    int y=*yp; // movq (%rsi) , %rcx
    int z=*zp;
    *yp=x;
    *zp=y;
    *xp=z;
}