#ifndef COMMON_TEST_H_
#define COMMON_TEST_H_
#include <assert.h>
#include <time.h>
#define MAXNODE 100
const char* phase;
clock_t start;
clock_t end;
double time_taken;

#define CheckConditon(cond) \
    if(!(cond)){ \
        fprintf(stderr,"%s:%d: %s: %s\n",__FILE__,__LINE__,phase,#cond);\
        abort();    \
    }   
#define StartPhase(name) \
    fprintf(stderr,"=== Test %s\n",name);\
    phase=name;



#endif