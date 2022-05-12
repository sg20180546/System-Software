#ifndef STOCKFILE_HANDLE_H_
#define STOCKFILE_HANDLE_H_
#include "common.h"

#include "binary_tree.h"

time_t t0;
time_t t1;
int time_check();

void read_stockfile();

void fsync_stockfile();
#endif