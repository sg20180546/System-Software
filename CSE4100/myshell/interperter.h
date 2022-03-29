#ifndef _INTERPRETER_H_
#define _INTERPERTER_H_
#define PIPE '|'
#define BACKGROUND '&'
#define SPACE ' '
#define ENTER 10

#include "type.h"
#include "common.h"

void interpreter(char* cmdline);
char* builtin_command_list[MAXARGS];

#endif