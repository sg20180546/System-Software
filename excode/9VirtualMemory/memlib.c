#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>

#include "../csapp.h"
#include "memlib.h"

#define MAX_HEAP (20*(1<<20))

static char* mem_heap;
static char* mem_brk;
static char* mem_max_addr;

void mem_init(void){
    mem_heap=(char*)Malloc(MAX_HEAP);
    mem_brk=(char*)mem_heap;
    mem_max_addr=(char*)(mem_heap+MAX_HEAP);
}

void * mem_sbrk(int incr){
    char* old_brk=mem_brk;
    if((incr<0)||((mem_brk+incr)>mem_max_addr)){
        errno=ENOMEM;
        fprintf(stderr,"ERROR: mem_Sbkr failed. ran out of memory\n");
    }
    mem_brk+=incr;
    return (void*)old_brk;
}