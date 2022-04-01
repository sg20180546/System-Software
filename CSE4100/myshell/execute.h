#ifndef _EXECUTE_H_
#define _EXECUTE_H_
#include "common.h"
#include "type.h"

int fds[2];
void execute_commands(struct command* first_cmd);

#endif