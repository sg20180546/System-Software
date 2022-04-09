#ifndef _JOBS_H_
#define _JOBS_H_


#include "../common.h"
#include "../type.h"
#include "../sig.h"

int jobs_front;
int jobs_rear;
int jobs_n;
// hasing name -> one time random access
// save current jobs number
// if garbage collection remove last job, init jobs number=0
JOB* jobs_list[MAXJOBS];
int jobs_done[MAXJOBS];
pid_t pid_list[MAXCMD];

void insert_jobs(pid_t pgid,char* cmdline,STATE state);

void print_jobs(char** argv);

void print_done_jobs();

void reap_dead_jobs();

int find_jobs_by_pid(pid_t pid);

// if bg, sigcont to bg process
void bg(char** argv);

// if fg, sigstop to main process, sigcont to bg process
void fg(char** argv);
#endif


