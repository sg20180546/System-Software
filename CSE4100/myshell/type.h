#ifndef _TYPE_H_
#include <unistd.h>
#include <stdio.h>
#include <signal.h>



typedef struct _bool{
 unsigned v:1;
}bool;

typedef enum _status{
    OK,BUFFERING,SYNTAXERR,NOCOMMANDERR
} status;

typedef enum _flag{
    STATIC,VARIABLE
}cmd_flag;


typedef struct _static_command{
    char* name;
    // void (*argument_checker)(char*,char*);
} static_command;


typedef struct _variable{
    char* key;
    char* value;
}variable;

typedef struct _command{
    char** arguments;
    cmd_flag f;
    command* redirectto;
    command* redirectfrom;
    bool is_redirecting;
    union
    {
        static_command* static_cmd;
        variable* variable
    };
} command;



variable** variable_list;

int num_variable=0;
int variable_list_size=1;



#endif