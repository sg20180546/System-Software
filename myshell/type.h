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
    ABSOLUTE,FUNCTION,RELATIVE
}cmd_flag;

typedef struct _builtin{
    char* name;
    void (*fp)(char**);
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
    union
    {
        builtin* builtin;
        variable* variable;
    };
    
};
typedef enum{
    RUNNING,SUSPENDED,TERMINATED
}STATE;



typedef struct _JOB{
    pid_t pgid;
    STATE state;
    char* cmdline;
}JOB;



variable** variable_list;





#endif