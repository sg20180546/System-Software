#include "common_test.h"
#include "../binary_tree.h"

short cur_node[MAXNODE+1];
int node_count=0;
// int i;
struct stock_info{
    int id, count,price;
};
struct stock_info info_arr[MAXNODE];
struct stock* node;
const char* phase;
int id,count,price;



int main(int argc,char** argv){
    start=clock();
    StartPhase("make _root node");
    {   
        id=53; count=44; price=3000;
        _root=insert(_root,id,count,price);
        cur_node[id]=1;
        
        info_arr[node_count].id=id;
        info_arr[node_count].count=count;
        info_arr[node_count].price=price;
        node_count++;
        assert(_root);
        CheckConditon(_root);
        CheckConditon(_root->id==id);
        CheckConditon(_root->count==count);
        CheckConditon(_root->price==price);
    }

    StartPhase("insert node id==78 at right,id==35 at left");
    {
        id=78;count=31;price=50000;
        _root=insert(_root,id,count,price);
        cur_node[id]=1;
        info_arr[node_count].id=id;
        info_arr[node_count].count=count;
        info_arr[node_count].price=price;
        node_count++;
        CheckConditon(_root->right->id==id);
        CheckConditon(_root->right->count==count)
        CheckConditon(_root->right->price==price)

        id=35;count=87;price=300;
        _root=insert(_root,id,count,price);
        cur_node[id]=1;
        info_arr[node_count].id=id;
        info_arr[node_count].count=count;
        info_arr[node_count].price=price;
        node_count++;
        CheckConditon(_root->left->id==id);
        CheckConditon(_root->left->count==count)
        CheckConditon(_root->left->price==price)
    }
    StartPhase("insert 98 nodes randomly and find node");
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
            insert(_root,id,count,price);
        }
        for(node_count=0;node_count<33;node_count++){
            struct stock_info info;
            info=info_arr[node_count];
            node=find(info.id,_root);
            
            assert(node);
            CheckConditon(info.count==node->count);
            CheckConditon(info.price==node->price);
        }
    }
    StartPhase("modify node");{
        int dx_arr[MAXNODE];
        int result[MAXNODE];
        for(node_count=0;node_count<MAXNODE;node_count++){
            int dx=(rand()%200)-(rand()%200);
            node=find(info_arr[node_count].id,_root);
            result[node_count]=node->count+dx;
            dx_arr[node_count]=dx;
        }
        for(node_count=0;node_count<MAXNODE;node_count++){

            STATUS st=modify(info_arr[node_count].id,dx_arr[node_count]);
            node=find(info_arr[node_count].id,_root);
            assert(node);
            if(result[node_count]>=0){
                CheckConditon(st==SUCCESS);
                CheckConditon(node->count==result[node_count]);
                info_arr[node_count].count=result[node_count];
            }else{
                CheckConditon(st==ERROR);
                CheckConditon(node->count!=result[node_count]);
                CheckConditon(node->count==info_arr[node_count].count);
            }
        }

    }
    // StartPhase("print to buffer");
    // {

    // }
    end=clock();
    time_taken= (end-start)/CLOCKS_PER_SEC;
    fprintf(stderr,"%s took %f seconds to execute\nPASS\n",argv[0]+2,time_taken);
    return 0;
}
