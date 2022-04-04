#ifndef _COMMON_H_
#define _COMMON_H_
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <wait.h>
#include <assert.h>
#define BUILTIN_PATH "../module/"
#define MAXARGS 16
#define MAXPROCESS 1024
#define BIN "/bin/"
#define	MAXLINE	4096
#define a(x) assert(x)
#define pd(x) printf("%s\n",(x))
pid_t parent_pid;


#endif