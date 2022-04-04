#ifndef _TYPE_H_
#define _TYPE_H_
#include "common.h"
#include "type.h"


extern int num_variable;
extern int variable_list_size;
typedef struct _bool{
 unsigned v:1;
}bool;

typedef enum _status{
    OK,BUFFERING,SYNTAXERR,NOCOMMANDERR
} status;

typedef enum _mode{
BACKGROUND,FOREGROUND
}MODE;

typedef enum _flag{
    STATIC,VARIABLE,CUSTOM,FUNCTION
}cmd_flag;


typedef struct _static_command{
    char* name;
    // void (*argument_checker)(char*,char*);
} static_command;

typedef struct _custom_command{
    char* name;
}custom_command;

typedef struct _builtin{
    char* name;
    status (*fp)(char*);
}builtin;

typedef struct _variable{
    char* key;
    char* value;
}variable;


struct command{
    unsigned short argc;
    char** arguments;
    cmd_flag f;
    struct command* redirectto;
    struct command* redirectfrom;
    bool is_redirecting;
    union
    {
        builtin* builtin;
        variable* variable;
    };
    
};



variable** variable_list;





#endif