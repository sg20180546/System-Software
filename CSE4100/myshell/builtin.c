#include "builtin.h"

status change_directory(char* path){
    status status=OK;
    return status;
}

struct command command_list[CMD_VARIABLE]={
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"cat"}},
    {0,NULL,FUNCTION,NULL,NULL,0,&(custom_command){"cd"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"clear"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"echo"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"exit"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"grep"}},
    {0,NULL,CUSTOM,NULL,NULL,0,&(static_command){"jobs"}},
    {0,NULL,CUSTOM,NULL,NULL,0,&(static_command){"kill"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"less"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"ls"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"mkdir"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"pwd"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"rmdir"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"sleep"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"touch"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"variable"}} };
