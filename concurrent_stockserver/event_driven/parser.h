#ifndef PARSER_H_
#define PARSER_H_
#include "common.h"
#include "impl.h"
STATUS find_cmd(char buf[],struct command* cmd,int* p);
STATUS find_args(char* buf,struct command* cmd);
STATUS parser(char* buf,int rc,struct command* cmd);
#endif