#include "binary_tree.h"


struct stock* insert(struct stock* root,int id,int count, int price){
    if(root==NULL){
        root=calloc(1,sizeof(struct stock));
        root->count=count;
        root->id=id;
        root->price=price;
        root->left=NULL; root->right=NULL;
        return root;
    }
    else if(root->id>id){
        root->left=insert(root->left,id,count,price);
    }else{
        root->right=insert(root->right,id,count,price);
    }

    return root;
}

void print_to_buf(struct stock* root,char* buf){
    if(root==NULL) return;
    char cat_buf[MAXLINE];
    sprintf(cat_buf,"%d %d %d\n",root->id,root->count,root->price);
    
    
    strcat(buf,cat_buf);
    print_to_buf(root->left,buf);
    print_to_buf(root->right,buf);
}


