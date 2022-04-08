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
#define BUILTIN_PATH "../module/"
#define MAXARGS 16
#define MAXJOBS 1024
#define MAXCMD 16
#define BIN "/bin/"
#define	MAXLINE	4096
#define a(x) assert(x)
#define pd(x) printf("%s\n",(x))
#define ff(x) fprintf(stderr,"%s\n",(x) );
pid_t parent_pid;
pid_t child_pid;
char buf[MAXLINE];
// pid_t child_pid2;
// pid_t son_pid;
// sigjmp_buf jbuf;
#endif