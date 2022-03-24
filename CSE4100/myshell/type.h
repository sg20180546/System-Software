#ifndef _TYPE_H_
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#define MAXARGS 128
#define	MAXLINE	 8192
typedef struct _bool{
 unsigned v:1;
} bool;

typedef struct _command{
    char* name;
    function* func;
} command;

typedef struct _function{
    char* source_file;
} function;


#endif