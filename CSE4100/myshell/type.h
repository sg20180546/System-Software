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

typedef enum _flag{
    STATIC,VARIABLE,CUSTOM
}cmd_flag;


typedef struct _static_command{
    char* name;
    // void (*argument_checker)(char*,char*);
} static_command;

typedef struct _custom_command{
    char* name;
}custom_command;

typedef struct _variable{
    char* key;
    char* value;
}variable;

typedef struct _command{
    unsigned short argc;
    char** arguments;
    cmd_flag f;
    struct _command* redirectto;
    struct _command* redirectfrom;
    bool is_redirecting;
    union
    {
        static_command* static_cmd;
        variable* variable;
        custom_command* cstm_cmd;
    };
} command;



variable** variable_list;





#endif