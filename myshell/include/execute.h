#ifndef _EXECUTE_H_
#define _EXECUTE_H_
#include "common.h"
#include "type.h"
#include "sig.h"
#include "jobs.h"
#define READ_END 0
#define WRITE_END 1
int fds[MAXCMD][2];
int fd;
char buf[MAXLINE];
void execute_commands(struct command* first_cmd);
void execute_function_command(struct command* func_cmd);
#endif