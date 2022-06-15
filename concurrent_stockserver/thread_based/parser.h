#ifndef PARSER_H_
#define PARSER_H_
#include "common.h"
#include "impl.h"

STATUS find_cmd(char buf[],struct connection* cnct,int* p);
STATUS find_args(char* buf,struct connection* cnct);
STATUS parser(char* buf,int rc,struct connection* cnct);
#endif