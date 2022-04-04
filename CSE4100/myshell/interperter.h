#ifndef _INTERPRETER_H_
#define _INTERPERTER_H_
#define PIPE '|'
#define AMPERSAND '&'
#define SPACE ' '
#define ENTER 10
#define TAB '\t'
#define whitespace(x) ( ( (x) ==SPACE) || ( (x) == TAB)  )


#include "type.h"
#include "common.h"
#include "execute.h"
#include "builtin.h"
#include "./module/jobs.h"
#include "./module/cd.h"

void interpreter(char* cmdline);
char* builtin_command_list[MAXARGS];



#endif