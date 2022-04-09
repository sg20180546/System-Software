#ifndef _COMMON_H_
#define _COMMON_H_
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <wait.h>
#include <assert.h>
#include <setjmp.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#define MAXARGS 16
#define MAXJOBS 1024
#define MAXCMD 16
#define	MAXLINE	4096
#define a(x) assert(x)


#define pf(x) printf("%s\n",(x))
#define ff(x) fprintf(stderr,"%s\n",(x) );

pid_t parent_pid;
pid_t child_pid;
sigset_t mask;
char buf[MAXLINE];
void safe_free(void * p);

#endif