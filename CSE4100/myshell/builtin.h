#ifndef _BUILTIN_H_
#define _BUILTIN_H_
#include "type.h"
#include "common.h"

// command* builtin_command;

#define CMD_VARIABLE 14

extern struct command command_list[CMD_VARIABLE];
extern int num_builtin_command;


#endif
