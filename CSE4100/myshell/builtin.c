#include "builtin.h"
#include "module/cd.h"
#include "module/jobs.h"
#include "module/exit.h"

struct command command_list[CMD_VARIABLE]={
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"bg",bg}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"cat"}},
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"cd",change_directory}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"clear"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"echo"}},
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"exit",exit_}},
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"fg",fg}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"grep"}},
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"jobs",print_jobs}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"kill"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"less"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"ls"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"mkdir"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"ps"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"pwd"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"rmdir"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"sleep"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"sort"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"touch"}},
    {0,NULL,VARIABLE,NULL,NULL,&(builtin){"variable"}} };
