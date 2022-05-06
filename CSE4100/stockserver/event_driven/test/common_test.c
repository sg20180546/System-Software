#include "common_test.h"



void StartPhase(const char* name){
    fprintf(stderr,"=== Test %s\n",name);
    phase=name;
}