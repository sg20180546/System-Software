#include "../binary_tree.h"
#include <assert.h>

#define MAXNODE 100
struct stock* root;
short cur_node[MAXNODE+1];
int node_count=0;
int i;
struct stock_info{
    int id, count,price;
};
struct stock_info info_arr[MAXNODE];
const char* phase;
int id,count,price;
static void StartPhase(const char* name){
    fprintf(stderr,"=== Test %s\n",name);\
    phase=name;
}
#define CheckConditon(cond) \
    if(!(cond)){ \
        fprintf(stderr,"%s:%d: %s: %s\n",__FILE__,__LINE__,phase,#cond);\
        abort();    \
    }   


int main(int argc,char** argv){
    StartPhase("make root node");
    {   
        id=5; count=44; price=3000;
        root=insert(root,id,count,price);
        cur_node[id]=1;
        
        info_arr[node_count].id=id;
        info_arr[node_count].count=count;
        info_arr[node_count].price=price;
        node_count++;
        assert(root);
        CheckConditon(root);
        CheckConditon(root->id==id);
        CheckConditon(root->count==count);
        CheckConditon(root->price==price);
    }

    StartPhase("insert node id=1,id=3");
    {
        id=15;count=31;price=50000;
        root=insert(root,id,count,price);
        cur_node[id]=1;
        info_arr[node_count].id=id;
        info_arr[node_count].count=count;
        info_arr[node_count].price=price;
        node_count++;
        CheckConditon(root->right->id==id);
        CheckConditon(root->right->count==count)
        CheckConditon(root->right->price==price)

        id=1;count=87;price=300;
        root=insert(root,id,count,price);
        cur_node[id]=1;
        info_arr[node_count].id=id;
        info_arr[node_count].count=count;
        info_arr[node_count].price=price;
        node_count++;
        CheckConditon(root->left->id==id);
        CheckConditon(root->left->count==count)
        CheckConditon(root->left->price==price)
    }
    StartPhase("insert 100 node randomly and find node");
    {
        for(;node_count<MAXNODE;node_count++){
            retry:
            id=rand()%MAXNODE;
            if(cur_node[id]) goto retry;
            count=rand()%100;
            price=rand()%1000000;
            cur_node[id]=1;

            info_arr[node_count].id=id;
            info_arr[node_count].count=count;
            info_arr[node_count].price=price;
            insert(root,id,count,price);
        }
        for(i=0;i<33;i++){
            struct stock_info info;
            struct stock* node;
            info=info_arr[i];
            node=find(info.id,root);
            
            assert(node);
            CheckConditon(info.count==node->count);
            CheckConditon(info.price==node->price);
        }
    }
    StartPhase("modify node");{

    }

}
