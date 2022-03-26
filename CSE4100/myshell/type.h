#ifndef _TYPE_H_
#include <unistd.h>
#include <stdio.h>
#include <signal.h>



typedef struct _bool{
 unsigned v:1;
} bool;

typedef struct _command{
    char* name;
    char* arguments;
    function* func;
} command;

typedef struct _function{
    char* source_file;
} function;

typedef enum _status{
    OK,SYNTAXERR,NOCOMMANDERR
} status;

typedef struct _variable{
    char* key;
    char* value;
}variable;

variable** variable_list;

int num_variable=0;
int variable_list_size=1;



#endif