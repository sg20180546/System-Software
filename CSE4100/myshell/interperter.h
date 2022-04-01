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

extern unsigned int running_background_jobs_front;
extern unsigned int running_background_jobs_rear;
char* running_background_jobs[MAXARGS];
void initialize();
void interpreter(char* cmdline);
char* builtin_command_list[MAXARGS];

#endif