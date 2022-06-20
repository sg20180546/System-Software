#include "builtin.h"
#include "module/cd.h"
#include "module/jobs.h"
#include "module/exit.h"

int num_command=29;
struct command command_list[]={
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"bg",bg}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"cat"}},
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"cd",change_directory}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"clear"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"code"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"echo"}},
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"exit",exit_}},
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"fg",fg}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"gcc"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"git"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"grep"}},
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"jobs",print_jobs}},
    {0,NULL,FUNCTION,NULL,NULL,&(builtin){"kill",kill_job}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"less"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"ls"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"make"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"mkdir"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"more"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"printenv"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"ps"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"pwd"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"rmdir"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"scp"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"sleep"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"sort"}},
     {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"ssh"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"tar"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"touch"}},
     {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"vi"}},
    {0,NULL,ABSOLUTE,NULL,NULL,&(builtin){"whereis"}},};
