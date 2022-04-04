#include "builtin.h"

status change_directory(char* path){
    status status=OK;
    return status;
}

struct command command_list[CMD_VARIABLE]={
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"cat"}},
    {0,NULL,FUNCTION,NULL,NULL,0,&(builtin){"cd",change_directory}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"clear"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"echo"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"exit"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"grep"}},
    {0,NULL,CUSTOM,NULL,NULL,0,&(builtin){"jobs"}},
    {0,NULL,CUSTOM,NULL,NULL,0,&(builtin){"kill"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"less"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"ls"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"mkdir"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"pwd"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"rmdir"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"sleep"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"touch"}},
    {0,NULL,STATIC,NULL,NULL,0,&(builtin){"variable"}} };
