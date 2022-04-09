#include "common.h"

void safe_free(void* p){
    if(p!=NULL) free(p);
}