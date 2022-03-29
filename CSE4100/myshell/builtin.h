#ifndef _TYPE_H_
#define _TYPE_H_
#include "type.h"
#include "common.h"

// command* builtin_command;

#define CMD_VARIABLE 10;

command command_list[]={
    {"cat",NULL,NULL},
    {"cd",NULL,NULL},
    {"clear",NULL,NULL},
    {"echo",NULL,NULL},
    {"exit",NULL,NULL},
    {"ls",NULL,NULL},
    {"mkdir",NULL,NULL},
    {"pwd",NULL,NULL},
    {"rmdir",NULL,NULL},
    {"touch",NULL,NULL},
    {"variable",NULL,NULL}
};

int num_builtin_command=9;


#endif
