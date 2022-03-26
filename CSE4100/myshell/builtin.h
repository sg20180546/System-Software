#include "type.h"
#include "common.h"

// command* builtin_command;

#define CMD_VARIABLE 10;

command command_list[]={
    {"cat",NULL},
    {"cd",NULL},
    {"clear",NULL},
    {"echo",NULL},
    {"exit",NULL},
    {"ls",NULL},
    {"mkdir",NULL},
    {"pwd",NULL},
    {"rmdir",NULL},
    {"touch",NULL},
    {"variable",NULL}
};

int num_builtin_command=9;


void initialize_command();

// {
//     DIR* streamp;
//     struct dirent* dep;

//     streamp=opendir(BUILTIN_PATH);
    

//     // how to make buitlin command array?
// }


