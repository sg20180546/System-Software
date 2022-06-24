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
    "Sungjin Byeon",
    /* Your email address */
    "sg20180546@gmail.com",
};

void noprintf(const char *__restrict __fmt, ...)
{

}

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
#define FREE_AREA_STRUCT_SIZE 12
#define PAGE_STRUCT_SIZE 16

#define BITMAP_INDEX(POS) ((POS)>>6)

#define GET_PAGE_N(addr) ((unsigned long)((addr)-(physical_memory_area))>>(PAGE_SHIFT))
#define GET_PAGE_n(addr) ((unsigned long)(((addr)-(page_struct_area))/PAGE_STRUCT_SIZE))
/* 
 * mm_init - initialize the malloc package.
 */
unsigned long phy_page_n;
unsigned long free_phy_page_n;
int page_struct_area_size;
int bitmap_area_size;
int free_area_area_size;
int total_mem_usage;
int row=0;
//16 byte
struct list_head{
    struct list_head *next,*prev;
};


// 12 byte
struct free_area
{
    struct list_head free_list;
    unsigned long* bitmap;
};
// 16 byte
struct page{
    struct list_head list;
    int order;
    void *physical_addr;
};
struct free_area *free_area_area;
struct page *page_struct_area;
void *bitmap_area,*physical_memory_area,*endpoint;

/*
--------------------------------------------list functions-----------------------------------------
bitmap_get/bitmap_set/bitmap_switch
*/
#define list_entry(ptr,type,member)\
                ((type*) ((char*)(ptr)-(unsigned long)(&((type*)0)->member) ))


//bug?
// static void list_add(struct free_area*area,struct page* p){
//     // area->free_list.next=&(p->list);
//     // assert(!p->list.next); assert(!p->list.prev);
//     // assert(area->free_list.next); assert(area->free_list.prev);
//     if(area->free_list.next) area->free_list.next->prev=&(p->list);
//     p->list.next=area->free_list.next;
//     if(area->free_list.prev) p->list.prev=&(area->free_list);
//     area->free_list.next=&(p->list);
//     assert(p->list.next); assert(area->free_list.prev);
//     assert(p->list.prev); assert(area->free_list.next);
// }

void list_add(struct list_head* head,struct list_head* new){
    __list_add(new,head,head->next);
}

void __list_add(struct list_head*new,struct list_head* prev,struct list_head *next){
    next->prev=new;
    new->next=next;
    new->prev=prev;
    prev->next=new;
}
static void list_del(struct list_head* l)
{
     
    // assert(l->prev);
    // assert(l->next);
    // if(l->prev) 
        l->prev->next=l->next;
    // if(l->next) 
        l->next->prev=l->prev;
    l->next=NULL;
    l->prev=NULL;
}
// void list_del(struct list_head * entry){
//     __list_del(entry->prev,entry->next);
//     entry->next=(void*)0;
//     entry->prev=(void*)0;
// }

// void __list_del(struct list_head*prev,struct list_head*next){
//     next->prev=prev;
//     prev->next=next;
// }
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
    // printf("physical mapping\n");
    for(int i=0;i<phy_page_n;i++)
    {   
        page_struct_area[i].physical_addr=offset;
        offset+=PAGE_SIZE;
        assert(page_struct_area[i].physical_addr);
        // if(i>phy_page_n-10) printf("physical area%p\n",page_struct_area[i].physical_addr);
        // ((struct page*)page_struct_area)[i].physical_addr=offset;
    }
    // printf("offset %p endpoint %p\n\n",offset,endpoint);
    assert(offset==endpoint);
}
static void mapping_bitmap()
{   
    assert(bitmap_area==(page_struct_area+phy_page_n));
    void*offset=bitmap_area;
    unsigned long bitmap_size=(phy_page_n>>1)>>3;
    for(int i=0;i<MAX_ORDER;i++)
    {   
        
        free_area_area[i].bitmap=offset;
        // ((struct free_area*)free_area_area)[i].bitmap=offset;
        assert(free_area_area[i].bitmap);
        memset(offset,0,bitmap_size|1);
        // printf("bitmap i addr : %d : %p %p \n",i,offset,free_area_area[i].bitmap);
        offset+=bitmap_size;
        if(bitmap_size>1) bitmap_size>>=1;
        
    }
    // printf("%p %p\n",offset,physical_memory_area);
    assert(offset<physical_memory_area);
    // printf("last bitmap : %p %p\n");
}

static void init_buddy()
{   
    
    struct free_area* area=free_area_area ;
    // struct free_area* temp=area;
    // printf("struct size %d page struct size %d\n",sizeof(struct free_area),sizeof(struct page));
    // printf("area : %p %p\n",area,free_area_area);
    // printf("%p %p %d %u %u\n",free_area_area,area,(void*)area-free_area_area,sizeof(struct free_area),sizeof(struct list_head*));
    for(int i=0;i<MAX_ORDER;i++){
        assert(area->bitmap);
        // assert(&area->free_list.next);
        area->free_list.next=&(area->free_list);
        area->free_list.prev=&(area->free_list);
        area++;
        // printf("gap %d\n",(void*)area-(void*)temp);
        // temp=area;
    }
    area=&free_area_area[MAX_ORDER-1];
    // printf("last area %p struct page %p gap %d \n",area,page_struct_area,(void*)page_struct_area-(void*)area);
    // printf("sig1\n");
    struct page* p0=page_struct_area;
    struct page* p1=page_struct_area+(2<<(MAX_ORDER-2));
    // printf("init budy ------------%d %d\n\n\n\n",GET_PAGE_N(p0->physical_addr),GET_PAGE_N(p1->physical_addr));
    // printf("\n\np0 phys %p pn %ld p1 phys %p pn %ld\n\n",p0->physical_addr,GET_PAGE_N(p0->physical_addr),p1,GET_PAGE_N(p1->physical_addr));
    // printf("sig2\n");
    list_add(&area->free_list,&p0->list);
    list_add(&area->free_list,&p1->list);
    assert(p0->list.next); assert(p0->list.prev);
    assert(p1->list.next); assert(p1->list.prev);
    // list_add(area,(struct page*)p0);
    // list_add(area,(struct page*)p1);
    assert(&area->free_list!=area->free_list.next);
    // printf("sig3 wqe\n");
    // printf("bitmap ?%p",area->bitmap);

    // assert((*area).bitmap);
    bitmap_set(area->bitmap,0,1);
    // printf("sig4\n");
}

/*
--------------------------------------------util functions-----------------------------------------
memsize_to_order/expand/alloc_pages
*/
static int memsize_to_order(size_t memsize)
{
    for(int i=0;i<=MAX_ORDER;i++){
        if(memsize<=(PAGE_SIZE<<i))
            return i;
    }
    return -1;
}
struct page* expand(struct page* page,unsigned long page_n,int high_order,int low_order,struct free_area* area)
{
    unsigned long size=1<<high_order;
    // printf("expand : page_n  %lu high order %d low order %d\n",page_n,high_order,low_order);
    ///// error point
    printf("high %d low %d\n",high_order,low_order);
    while(high_order>low_order)
    {
        // not deleteing ??
        printf("%p %d ",page->physical_addr,GET_PAGE_N(page->physical_addr));
        area--;
        high_order--;
        size>>=1;
        // printf("%ld ")
        // list_add(area,page);
        list_add(&(area)->free_list,&(page)->list);
        assert(page->list.next); assert(page->list.prev);
        // printf("expand: page_idx %ld\n",GET_PAGE_n(page));
        MARK_USED(page_n,high_order,area);
        page_n+=size;
        page+=size;
        
    }
    printf("\nexpand: %p page_idx %ld\n",page->physical_addr,GET_PAGE_N(page->physical_addr));
    return page;
}
void* alloc_pages(int order)
{   
    // printf("order : %d\n",order);
    if(order==-1) return NULL;
    struct page* page;
    unsigned int curr_order=order;
    struct free_area* area=&free_area_area[order];
    struct list_head* head,*curr;
//     expand: 0xf65e30d8 page_idx 65520
// allocated addr 0xf65e30d8 index 65520
// -----------------------------------

// curr order 0  head 0xf54e2010 area_struct :0xf54e2010 curr 0xf54e2010
// curr order 1  head 0xf54e201c area_struct :0xf54e201c curr 0xf54e201c
// curr order 2  head 0xf54e2028 area_struct :0xf54e2028 curr 0xf54e2028
// curr order 3  head 0xf54e2034 area_struct :0xf54e2034 curr 0xf55e1fd0
// high 3 low 0
// 0xf65e30d8 65520 0xf65e34d8 65524 0xf65e36d8 65526 
// expand: 0xf65e37d8 page_idx 65527

    do
    {
        head=&area->free_list;
        curr=head->next;
        printf("curr order %d  head %p area_struct :%p curr %p\n",curr_order,area,head,curr);
        assert(curr); assert(head); assert(area);
        if(head!=curr)
        {   
            
            page=list_entry(curr,struct page,list);
            assert(page); assert(page->physical_addr);
            
            list_del(curr);
            assert(head->next!=curr);
            
            // printf("page %p INDEX %d\n",page,pn);
            // assert(page);
            // assert(page->physical_addr);
            unsigned long page_n=GET_PAGE_N(page->physical_addr);
            // assert(pn==page_n);
            // printf("%d ",)
            if(curr_order!=MAX_ORDER-1) 
                MARK_USED(page_n,curr_order,area);
            free_phy_page_n=free_phy_page_n-(1UL<<order);
            assert(page->physical_addr);
            // printf("\nphys addr %p\n\n\n",page->physical_addr);
            page=expand(page,page_n,curr_order,order,area);
            page->order=order;
            free_phy_page_n-=1UL<<order;
            // printf
            // error
            assert(GET_PAGE_N(page->physical_addr)<65537);
            printf("allocated addr %p index %d\n-----------------------------------\n\n",page->physical_addr,GET_PAGE_N(page->physical_addr));
            return page->physical_addr;
        }
        curr_order++;
        area++;
       
    } while (curr_order<MAX_ORDER);
    // printf("aloc faile d heare\n");
    return NULL;
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
    page_struct_area_size=sizeof(struct page)*phy_page_n;
    bitmap_area_size=(phy_page_n/8)+4;
    free_area_area_size=sizeof(struct free_area)*MAX_ORDER;
    total_mem_usage=PHYSICAL_MEMORY_SIZE+free_area_area_size+bitmap_area_size+page_struct_area_size;

    free_area_area=mem_sbrk(free_area_area_size);
    page_struct_area=mem_sbrk(page_struct_area_size);
    bitmap_area=mem_sbrk(bitmap_area_size);
    unsigned long align=(unsigned long)bitmap_area+bitmap_area_size;
    align=(unsigned long) (7&~(align&7))+1UL;
    mem_sbrk(align);
    // printf("\nalign : %u algin p %p\n",align,align_p);
    physical_memory_area=mem_sbrk(PHYSICAL_MEMORY_SIZE);
    // mem_reset_brk();
    // printf("hello");
    // printf("\nphy_page_n %ld\npage_struct area size %d\nbitmap area size %d\nfree area area size%d\n",phy_page_n,page_struct_area_size,bitmap_area_size,free_area_area_size);
    // printf("total mem usage %d\n",total_mem_usage);
    // printf("unused mem : %d",MAX_HEAP-total_mem_usage);
    endpoint=physical_memory_area+PHYSICAL_MEMORY_SIZE;
    assert((void*)free_area_area+FREE_AREA_STRUCT_SIZE*MAX_ORDER==page_struct_area);
    assert((void*)page_struct_area+PAGE_STRUCT_SIZE*phy_page_n ==bitmap_area);
    // printf("\nfree area area: %p\npage_strct_area: %p\nbitmap_area: %p\nphysical_memory_area: %p\nend:%p\n",free_area_area,page_struct_area,bitmap_area,physical_memory_area);
    // printf("%d %d %d %d",page_struct_area-free_area_area,bitmap_area-page_struct_area,physical_memory_area-bitmap_area,endpoint-physical_memory_area);
    mapping_physical();
    mapping_bitmap();
    // printf("mapping\n");
    init_buddy();
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    // printf("malloc row : %d\n",row++);
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
    // printf("free row : %d\n",row++);
    unsigned long page_n=GET_PAGE_N(ptr);
    struct page page=page_struct_area[page_n];

    unsigned long order=page.order;
    struct free_area* area=&free_area_area[order];

    unsigned long bit_index=page_n>>(1+order);
    unsigned long mask=(~0UL)<<order;
    // printf("free : %p page n %u order : %u bit index %u\n",ptr,page_n,order,bit_index);
    while(area<page_struct_area)
    {
        struct page buddy;
        if((void*)area>=(void*)page_struct_area){
            printf("over free_area boundry");
            break;
        }
        if(!bitmap_switch(area->bitmap,bit_index))
        {
            break;
        }
        unsigned long buddy_idx=page_n^(1<<order);
        buddy=page_struct_area[buddy_idx];
        printf("merge : page_n : %d buddy_idx %d order %d\n",page_n,buddy_idx,order);
        // assert(buddy.list.next); assert(buddy.list.prev);
        #ifdef DEBUG
            struct list_head* l=&area->free_list;
            while(l->next!=&area->free_list)
            {   
                struct page* p=list_entry((area->free_list.next),struct page,list);
                printf("index n %ld ",GET_PAGE_N(p->physical_addr));
                l=l->next;
            }
            printf("\n");
        #endif
        assert(&free_area_area[order].free_list!=buddy.list.next);
        assert(&free_area_area[order].free_list!=buddy.list.prev);
        assert(buddy.list.next); assert(buddy.list.prev);
        list_del(&buddy.list);
        mask<<=1;
        area++;
        order++;
        bit_index>>=1;
        page_n &= (~0 << order);
    }
    
    // list_add(area,&page_struct_area[page_n]);
    list_add(&area->free_list,&page_struct_area[page_n].list);
    assert(page_struct_area[page_n].list.prev); assert(page_struct_area[page_n].list.next);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{   
    // printf("realloc row : %d\n",row++);
    void* new=mm_malloc(size);
    memcpy(new,ptr,size);
    mm_free(ptr);
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
    return new;
}
