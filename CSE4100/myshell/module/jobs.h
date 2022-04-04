#ifndef _JOBS_H_
#define _JOBS_H_


#include "../common.h"
#include "../type.h"

extern int p_front;
extern int p_rear;

// hasing name -> one time random access
// save current jobs number
// if garbage collection remove last job, init jobs number=0
PROCESS process_group_list[MAXPROCESS];


void insert_jobs(pid_t pgid,char* cmdline,int foreground);

void print_jobs(char** argv);

void garbage_collection();


// if bg, sigcont to bg process
void bg(char** argv);

// if fg, sigstop to main process, sigcont to bg process
void fg(char** argv);
#endif


