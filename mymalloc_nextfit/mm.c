/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"
#include <pthread.h>
/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your information in the following struct.
 ********************************************************/
team_t team = {
    /* Your student ID */
    "20180546",
    /* Your full name*/
    "Sungjin Byeon",
    /* Your email address */
    "sg20180546@gmail.com",
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 
 * mm_init - initialize the malloc package.
 */
#define WSIZE 4
#define DSIZE 8
#define CHUNCKSIZE (1<<12)

#define MAX(x,y) ((x)>(y) ? (x): (y))

#define PACK(size, alloc) ((size)|(alloc))

#define GET(p) (*(unsigned int*)(p))
#define PUT(p,val) (*(unsigned int*)(p)=(val))

#define GET_SIZE(p) (GET(p)&~0x7)
#define IS_IT_ALLOC(p) (GET(p)&0x1)

#define HEADER_P(bp) ((char*)(bp)-WSIZE)
#define FOOTER_P(bp) ((char*)(bp)+GET_SIZE(HEADER_P(bp))-WSIZE-WSIZE)

// #define NEXT_BLOCK_P(bp) (((char*)(bp)-WSIZE+GET_SIZE((char*)(bp)) ))
#define NEXT_BLOCK_P(bp) (((char*)(bp)+GET_SIZE(HEADER_P(bp)) ))
#define PREV_BLOCK_P(bp) (((char*)(bp)-GET_SIZE((char*)(bp)-DSIZE) ))
// #define PREV_BLOCK_P(bp) (((char*)(bp)-WSIZE-GET_SIZE((char*)(bp)-DSIZE) ))
    // size_t prev_alloc=IS_IT_ALLOC(FOOTER_P((PREV_BLOCK_P(bp))));
static void* heap_listp;
static char* last_bp;
static void* extend_heap(size_t words);
static void* coalesce(void* bp);
static void *next_fit(size_t adjusted_size);
static void mark(void* bp,size_t adjusted_size);


int mm_init(void)
{   
    if((heap_listp=mem_sbrk(4*WSIZE))==(void*)-1)
        return -1;
    PUT(heap_listp,0);
    PUT(heap_listp+(1*WSIZE),PACK(DSIZE,1));
    PUT(heap_listp+(2*WSIZE),PACK(DSIZE,1));
    PUT(heap_listp+(3*WSIZE),PACK(0,1));
    heap_listp+=WSIZE<<1;

    if(extend_heap(CHUNCKSIZE/WSIZE)==NULL)
        return -1;

    last_bp=(char*)heap_listp;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t adjusted_size,extended_size;
    char* bp;
    if(size==0) return NULL;

    // 4byte / 8byte / 4byte
    if(size<=DSIZE) 
        adjusted_size=2*DSIZE;
    else 
        adjusted_size=DSIZE*((size+(DSIZE+DSIZE-1) )/DSIZE);

    if((bp=next_fit(adjusted_size)))
    {
        mark(bp,adjusted_size);
        last_bp=bp;
        return bp;
    }
    extended_size=MAX(adjusted_size,CHUNCKSIZE);
// https://dapsu-startup.tistory.com/entry/Malloc-Lab-%EB%8F%99%EC%A0%81-%EB%A9%94%EB%AA%A8%EB%A6%AC-%ED%95%A0%EB%8B%B93-Implicit-first-fit-next-fit-%EC%BD%94%EB%93%9C-%EA%B5%AC%ED%98%84
    if(!(bp=extend_heap(extended_size/WSIZE)) )
    {
        return NULL;
    }
    mark(bp,adjusted_size);
    last_bp=bp;
    return bp;

}
// HEADER(4byte) / (bp)PAYLOAD / FOOTER(4byte)


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size= GET_SIZE(HEADER_P(ptr));
    PUT(HEADER_P(ptr),PACK(size,0));
    PUT(FOOTER_P(ptr),PACK(size,0));
    coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    size_t old_size=GET_SIZE(HEADER_P(ptr));
    size_t new_size=size+(WSIZE<<1);
    if(new_size<=old_size)
    {
        return ptr;
    }
    else
    {
        size_t is_next_alloc=IS_IT_ALLOC(HEADER_P(NEXT_BLOCK_P(ptr)));
        size_t current_size=old_size+GET_SIZE(HEADER_P(NEXT_BLOCK_P(ptr)));
        if(!is_next_alloc&&current_size>=new_size)
        {
            PUT(HEADER_P(ptr),PACK(current_size,1));
            PUT(FOOTER_P(ptr),PACK(current_size,1));
            return ptr;
        }
        else
        {
            void* new_bp=mm_malloc(new_size);
            mark(new_bp,new_size);
            memcpy(new_bp,ptr,new_size);
            mm_free(ptr);
            return new_bp;
        }
    }
}


static void mark(void*bp,size_t adj_size)
{
    size_t cur_size=GET_SIZE(HEADER_P(bp));
    // divide 
    if((cur_size-adj_size)>=(DSIZE<<1) )
    {
        PUT(HEADER_P(bp),PACK(adj_size,1));
        PUT(FOOTER_P(bp),PACK(adj_size,1));

        bp=NEXT_BLOCK_P(bp);
        PUT(HEADER_P(bp),PACK(cur_size-adj_size,0));
        PUT(FOOTER_P(bp),PACK(cur_size-adj_size,0));
    }else{ //use as it is
        PUT(HEADER_P(bp),PACK(cur_size,1));
        PUT(FOOTER_P(bp),PACK(cur_size,1));
    }
}

static void*extend_heap(size_t words)
{
    char* bp;
    size_t size;

    size=(words%2) ?  (words+1)<<2: words<<2;

    if((long)(bp=mem_sbrk(size) )==-1)
        return NULL;

    PUT(HEADER_P(bp),PACK(size,0));
    PUT(FOOTER_P(bp),PACK(size,0));
    PUT(HEADER_P(NEXT_BLOCK_P(bp)),PACK(0,1));
    
    return coalesce(bp);
}

static void* coalesce(void* bp)
{
    size_t is_prev_alloc=IS_IT_ALLOC(FOOTER_P((PREV_BLOCK_P(bp))));
    size_t is_next_alloc=IS_IT_ALLOC(HEADER_P((NEXT_BLOCK_P(bp))));
    size_t size=GET_SIZE(HEADER_P(bp));

    if(is_prev_alloc&&is_next_alloc)
    {
        last_bp=bp;
        return bp;
    }
    else if(is_prev_alloc&&!is_next_alloc)
    {
        size+=GET_SIZE(HEADER_P(NEXT_BLOCK_P(bp)));
        PUT(HEADER_P(bp),PACK(size,0));
        PUT(FOOTER_P(bp),PACK(size,0));
    }
    else if(!is_prev_alloc&&is_next_alloc)
    {
        size+=GET_SIZE(HEADER_P(PREV_BLOCK_P(bp)));
        PUT(FOOTER_P(bp),PACK(size,0));
        PUT(HEADER_P(PREV_BLOCK_P(bp)),PACK(size,0));
        bp=PREV_BLOCK_P(bp);
    }else
    {
        size+=GET_SIZE(HEADER_P(NEXT_BLOCK_P(bp)))+GET_SIZE(HEADER_P(PREV_BLOCK_P(bp)));
        PUT(HEADER_P(PREV_BLOCK_P(bp)),PACK(size,0));
        PUT(FOOTER_P(NEXT_BLOCK_P(bp)),PACK(size,0));
        bp=PREV_BLOCK_P(bp);
    }
    last_bp=bp;
    return bp;
}

static void* next_fit(size_t adj_size)
{
    char* bp=last_bp;
    for(bp=NEXT_BLOCK_P(bp);GET_SIZE(HEADER_P(bp))!=0;bp=NEXT_BLOCK_P(bp) )
    {   
        void* hp =HEADER_P(bp);
        if(!IS_IT_ALLOC(hp)&&GET_SIZE(hp)>adj_size)
        {
            last_bp=bp;
            return bp;
        }
    }

    // from first heap_list
    bp=heap_listp;
    while(bp<last_bp)
    {
        bp=NEXT_BLOCK_P(bp);
        void* hp =HEADER_P(bp);
        if(!IS_IT_ALLOC(hp)&&GET_SIZE(hp)>=adj_size )
        {
            last_bp=bp;
            return bp;
        }
    }
    return NULL;
}