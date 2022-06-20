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
#include <stdbool.h>
#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your information in the following struct.
 ********************************************************/
team_t team = {
    /* Your student ID */
    "20180546",
    /* Your full name*/
    "Seungjin Lee",
    /* Your email address */
    "seungjinn@sogang.ac.kr",
};


/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
// #define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


// #define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

//////////////////////////////////////////
#define MAX_HEAP (20*(1<<20))  /* 20 MB */
#define PHYSICAL_MEMORY_SIZE (1UL<<24)
#define PAGE_SHIFT 8
#define PAGE_SIZE (1UL<<PAGE_SHIFT)

#define BUDDY_MAX_PAGESIZE (PAGE_SIZE<<MAX_ORDER)
#define TOTAL_PAGES(MEMSIZE) (MEMSIZE>>PAGE_SHIFT)
#define MAX_ORDER (24-PAGE_SHIFT)
#define FREE_AREA_STRUCT_SIZE 24
#define PAGE_STRUCT_SIZE 32

#define BITMAP_INDEX(POS) ((POS)>>6)

#define GET_PAGE_N(addr) ((unsigned long)((addr)-(physical_memory_area))>>(PAGE_SHIFT))
/* 
 * mm_init - initialize the malloc package.
 */
unsigned long phy_page_n;
unsigned long free_phy_page_n;
int page_struct_area_size;
int bitmap_area_size;
int free_area_area_size;
int total_mem_usage;
void *free_area_area,*page_struct_area,*bitmap_area,*physical_memory_area,*end;


//16 byte
struct list_head{
    struct list_head *next,*prev;
};


// 24 byte
struct free_area
{
    struct list_head free_list;
    unsigned long* bitmap;
};
// 32 byte
struct page{
    struct list_head list;
    int order;
    void *physical_addr;
};
/*
--------------------------------------------list functions-----------------------------------------
bitmap_get/bitmap_set/bitmap_switch
*/
#define list_entry(ptr,type,member)\
                ((type*) ((char*)(ptr)-(unsigned long)(&((type*)0)->member) ))

static void list_add(struct free_area*area,struct page* p){
    area->free_list.next=&(p->list);
    p->list.next=area->free_list.next;
    p->list.prev=&(area->free_list);
    area->free_list.next=&(p->list);
}
static void list_del(struct list_head* l)
{
    l->prev->next=l->next;
    l->next->prev=l->prev;
    l->next=(void*)0;
    l->prev=(void*)0;
}
/*
--------------------------------------------bitmap functions-----------------------------------------
bitmap_get/bitmap_set/bitmap_switch
*/
static bool bitmap_get(unsigned long* bitmap,int pos){
    int index=BITMAP_INDEX(pos);

    return ( (bitmap[index])& (1UL<<(pos&31)));
} 

static bool bitmap_set(unsigned long *bitmap,int pos, bool set)
{
    int index=BITMAP_INDEX(pos);
    bool prev=bitmap_get(bitmap,pos);
    if(set) bitmap[index]|=(1UL<<(pos&31));
    else bitmap[index]&= (~0UL^(1UL<<(pos%32) )); // 11111111 ^ 0000001=11111110
    return prev;
}

static bool bitmap_switch(unsigned long* bitmap,int pos)
{
    bool prev=bitmap_get(bitmap,pos);
    bitmap_set(bitmap,pos,!prev);
    return prev;
}
#define MARK_USED(index, order, area)\
                            bitmap_switch((area)->bitmap,(index)>>(1+(order)))
/*
--------------------------------------------initializing functions-----------------------------------------
mapping_physical/mapping_bitmap/init_buddy
*/
static void mapping_physical(){
    void* offset=physical_memory_area;
    printf("physical mapping\n");
    for(int i=0;i<phy_page_n;i++)
    {   
        ((struct page*)page_struct_area)[i].physical_addr=offset;
    }
}
static void mapping_bitmap()
{
    void*offset=bitmap_area;
    unsigned long bitmap_size=(phy_page_n>>1)>>3;
    for(int i=0;i<MAX_ORDER;i++)
    {   
        
        ((struct free_area*)free_area_area)[i].bitmap=offset;
        memset(offset,0,bitmap_size);
        printf("bitmap i addr : %d : %p \n",i,offset);
        offset+=bitmap_size;
        if(bitmap_size>1) bitmap_size>>=1;
        
    }
    // printf("last bitmap : %p %p\n");
}

static void init_buddy()
{   
    
    struct free_area* area=&((struct free_area*)free_area_area)[MAX_ORDER-1] ;
    printf("%p %p %d %u %u\n",free_area_area,area,(void*)area-free_area_area,sizeof(struct free_area),sizeof(struct list_head*));
    struct page* p0=page_struct_area;
    struct page* p1=page_struct_area+PAGE_STRUCT_SIZE*(2<<(MAX_ORDER-1));
    list_add(area,(struct page*)p0);
    list_add(area,(struct page*)p1);
    bitmap_set(area->bitmap,0,1);
}

/*
--------------------------------------------util functions-----------------------------------------
memsize_to_order/alloc_pages/expand
*/
static int memsize_to_order(size_t memsize)
{
    for(int i=0;i<=MAX_ORDER;i++){
        if(memsize<=(PAGE_SIZE<<i))
            return i;
    }
    return -1;
}

void* alloc_pages(int order)
{
    struct page* page;
    unsigned int curr_order=order;
    struct free_area* area=(struct free_area*)&free_area_area[order];
    struct list_head* head,*curr;
    do
    {
        head=&area->free_list;
        curr=head->next;
        if(head!=curr)
        {
            page=list_entry(curr,struct page,list);
            list_del(curr);
            unsigned long page_n=GET_PAGE_N(page->physical_addr);
            if(curr_order!=MAX_ORDER-1) MARK_USED(page_n,curr_order,area);
            free_phy_page_n=free_phy_page_n-(1UL<<order);
            // page=expand(page,page_n,order,curr_order,area);
            page->order=order;
            return page->physical_addr;
        }
        curr_order++;
        area+=FREE_AREA_STRUCT_SIZE;
    } while (curr_order<MAX_ORDER);
    return NULL;
}
struct page* expand(struct page* page,unsigned long page_n,int high_order,int low_order,void* area)
{

}
// 128 byte page // 2^16 physical area
/* 
0       1   2     3     4   5   6      7    8    9
128     256    512  1024 2048 4096 8192 16384 32768 65366
65536   32768 16384 8192 4096 2048 1024 512   256   128 64(9) // 32(10) 16 8  4  2  1
*/

// 256 byte page // 2^16 physical area
// 384 // 2621440/2 // 8192 // 16777216
// order: 0     1      2   3    4   5     6    7     8   9        10    11 12 13 14 15
//Psize: 256    512  1024 2048 4096 8192 16384 32768 65366
// bit : 32768 16384 8192 4096 2048 1024 512   256   128 64(9) // 32(10) 16 8  4  2  1
// byte : 4096 2048  1024 512  256  128  64    32    16  8  //    4      2  1  1  1  1



int mm_init(void)
{   
    mem_init();
    phy_page_n=free_phy_page_n=TOTAL_PAGES(PHYSICAL_MEMORY_SIZE);
    page_struct_area_size=PAGE_STRUCT_SIZE*phy_page_n;
    bitmap_area_size=phy_page_n/8;
    free_area_area_size=FREE_AREA_STRUCT_SIZE*MAX_ORDER;
    total_mem_usage=PHYSICAL_MEMORY_SIZE+free_area_area_size+bitmap_area_size+page_struct_area_size;

    free_area_area=mem_sbrk(free_area_area_size);
    page_struct_area=mem_sbrk(page_struct_area_size);
    bitmap_area=mem_sbrk(bitmap_area_size);
    physical_memory_area=mem_sbrk(PHYSICAL_MEMORY_SIZE);
    // printf("hello");
    printf("%d\n%d\n%d\n%d\n",phy_page_n,page_struct_area_size,bitmap_area_size,free_area_area_size);
    // printf("total mem usage %d\n",total_mem_usage);
    // printf("unused mem : %d",MAX_HEAP-total_mem_usage);
    end=mem_sbrk(1);

    printf("free area area: %p\npage_strct_area: %p\nbitmap_area: %p\nphysical_memory_area: %p\nend:%p\n",free_area_area,page_struct_area,bitmap_area,physical_memory_area);
    printf("%d %d %d %d",page_struct_area-free_area_area,bitmap_area-page_struct_area,physical_memory_area-bitmap_area,end-physical_memory_area);
    mapping_physical();
    mapping_bitmap();

    init_buddy();
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int order=memsize_to_order(size);
    void* addr=alloc_pages(order);
    if(!addr) printf("page allocation falied\n");
    return addr;
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{

}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    // allocate
    // copy
    // free

    // void *oldptr = ptr;
    // void *newptr;
    // size_t copySize;
    
    // newptr = mm_malloc(size);
    // if (newptr == NULL)
    //   return NULL;
    // // copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    // if (size < copySize)
    //   copySize = size;
    // memcpy(newptr, oldptr, copySize);
    // mm_free(oldptr);
    // return newptr;
}
