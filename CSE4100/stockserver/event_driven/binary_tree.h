#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_
#include "common.h"

struct stock* insert(struct stock* root,int id,int count,int price);

void modify(int id,int count);

void print_to_buf(struct stock* root,char* buf);
#endif