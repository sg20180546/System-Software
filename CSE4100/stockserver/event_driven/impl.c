#include "impl.h"





STATUS show(char* buf){
    return print_to_buf(_root,buf);
}

STATUS sell(int id,int count){
    return modify(id,count);
}

STATUS buy(int id, int count){
    return modify(id,-count);
}


