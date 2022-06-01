#define MAX_ORDER 10
#define BLOCK_SIZE 8;
unsigned long bitmap[MAX_ORDER];

struct node{
    void* pointer;
    struct node* next;
    struct node* prev;
};

struct free_area{
    struct node head;
    unsigned long* map;
};

struct buddy{
    struct free_area free_area[MAX_ORDER];
    // struct buddy *next_buddy,*prev_buddy;
};