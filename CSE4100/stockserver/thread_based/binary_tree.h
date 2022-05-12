#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_
#include "common.h"

struct stock* insert(struct stock* root,int id,int count,int price);
struct stock* find(int id,struct stock* root);
STATUS modify(int id,int count);

STATUS print_to_buf(struct stock* root,char* buf);
#endif