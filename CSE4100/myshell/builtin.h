#ifndef _BUILTIN_H_
#define _BUILTIN_H_
#include "type.h"
#include "common.h"

// command* builtin_command;

#define CMD_VARIABLE 11

command command_list[]={
    {0,NULL,STATIC,NULL,NULL,NULL,{"cat"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"cd"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"clear"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"echo"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"exit"}},
    {0,NULL,CUSTOM,NULL,NULL,NULL,{"jobs"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"ls"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"mkdir"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"pwd"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"rmdir"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"touch"}},
    {0,NULL,STATIC,NULL,NULL,NULL,{"variable"}}
};

int num_builtin_command=CMD_VARIABLE-1;


#endif
