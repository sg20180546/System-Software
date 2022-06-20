#include <unistd.h>

void mem_init(void);
void* mem_sbrk(int incr);
void mem_deinit(void);
void* mem_heap_lo(void);
void* mem_help_hi(void);
size_t mem_heapsize(void);
size_t mem_pagesize(void);