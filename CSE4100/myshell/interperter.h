#ifndef _INTERPRETER_H_
#define _INTERPERTER_H_
#define PIPE '|'
#define BACKGROUND '&'
#define SPACE ' '
#define ENTER 10

#define whitespace(x) ( ( (x) ==' ')|| ( (x) = '\t')  )


#include "type.h"
#include "common.h"

unsigned int running_background_jobs_front;
unsigned int running_background_jobs_rear;
char* running_background_jobs[MAXARGS];
void initialize();
void interpreter(char* cmdline);
char* builtin_command_list[MAXARGS];

#endif