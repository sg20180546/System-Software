short scale3(short x, short y, short z){
    // %rbx=10y
    // %rbx=10y+z
    // %rbx=10y+z+x*y
    short t=10*y+z+x*y;
    return t;
}