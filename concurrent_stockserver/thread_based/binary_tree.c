#include "binary_tree.h"


struct stock* insert(struct stock* root,int id,int count, int price){
    if(root==NULL){
        root=calloc(1,sizeof(struct stock));
        root->count=count;
        root->id=id;
        root->price=price;
        root->left=NULL; root->right=NULL;
        sem_init(&root->modify_mutex,0,1);
        return root;
    }
    else if(root->id>id){
        root->left=insert(root->left,id,count,price);
    }else{
        root->right=insert(root->right,id,count,price);
    }

    return root;
}

STATUS print_to_buf(struct stock* root,char* buf){
    if(root==NULL) return ERROR;
    char cat_buf[MAXLINE];
    sprintf(cat_buf,"%d %d %d\n",root->id,root->count,root->price);
    
    
    strcat(buf,cat_buf);
    print_to_buf(root->left,buf);
    print_to_buf(root->right,buf);
    return SUCCESS;
}

STATUS modify(int id,int count){
    struct stock* stock=find(id,_root);
    
    if(stock==NULL){
        return ERROR;
    } 
    if((stock->count+count)<0){
        return NOTENOUGHERR;
    }
    sem_wait(&stock->modify_mutex);
    stock->count+=count;
    sem_post(&stock->modify_mutex);
    return SUCCESS;
}
struct stock* find(int id,struct stock* root){
    struct stock* node;
    if(root==NULL) node=root;
    else if(id==root->id) node=root;
    else if(id>root->id) node=find(id,root->right);
    else node=find(id,root->left);
    return node;
}


