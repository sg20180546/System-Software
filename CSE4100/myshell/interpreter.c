#include "interperter.h"
#include "type.h"
#include "builtin.h"
#include "common.h"
#include "execute.h"


// #include "myshell.h"
static void parser();

/*
FOREGROUND EXECUTING - suspend parent process, until forked child process send SIGCHILD

BACKGROUDN EXECUTING - parent do parent, child do child
*/



void interpreter(char* cmdline,char* bgbuffer){
    status err;
    command command_execute_list[MAXARGS];
    unsigned short pipen=0;
    bool is_background;
    char* buf;
    strcpy(buf,cmdline);
    parser(buf,command_execute_list,pipen,&is_background,&err);
 
        switch (err)
        {
        case OK:
            if(is_background.v){
                execute_commands(command_execute_list,pipen);
                return;
            }
            execute_commands(command_execute_list,pipen);
            while(waitpid(NULL,NULL,NULL)>0);
            break;
        case SYNTAXERR:
            break;

        case NOCOMMANDERR:
            break;
        default:
            break;
        }
}

// find builtin name by binary search
// need to make builtin main function, excecutable object file
// builtin list-> in complile time? run time?
static void parser(char* cmdline,command* cmd_exec_ls,unsigned short* pipen,bool* is_background,status* err){
    unsigned int i=0;
    while(cmdline[i++]!=ENTER){
        while(cmdline[i++]!=SPACE);
        unsigned int j=i;

        while( (cmdline[j++]!=SPACE));

        command* cbuf=find_shell_command(&cmdline[j]);
        if(cbuf==NULL){
            *err=NOCOMMANDERR;
            return;
        }
        cmd_exec_ls[*pipen]=*cbuf;


        find_arguments(cbuf,&cmdline[i],pipen);

        while(cmdline[i++]!=SPACE)

        if(cmdline[i]==PIPE) *pipen++;




        // how to check and save command efficeintly, not using strncmp? -> binary search
        // pipeline -> how to write other proccess file descriptor 0 of file table, whcih is stdin?
        // |-> because child copy same file table with parent, writing to 1 is can wathced in other process
        i=j;
        //binarysearch

        // if not found , found abc=test
        

        // if not unfound command

    }
}

static command* find_shell_command(char* cmdline){
        unsigned short low=0,high=num_builtin_command,mid;
        command* cmd=NULL;
        bool is_static_command;
        is_static_command.v=0;
        while(low<=high){
            mid=(low+high)/2;

            int c=cmdline[0]-command_list[mid].static_cmd->name[0];

            if(c==0){
              if(!strncmp(cmdline,command_list[mid].static_cmd->name,sizeof(command_list[mid].static_cmd->name))){
                  cmd=malloc(sizeof(command));
                  cmd->f=STATIC;
                  cmd->static_cmd=malloc(sizeof(static_command));
                  strcpy(cmd->static_cmd->name,command_list[mid].static_cmd->name);
                  is_static_command.v=1;
              }  
            } 
            else if(c>0) low=mid+1;
            else high=mid-1;

        }
        
        if(cmd==NULL){
            cmd=is_variable(cmdline);
        }

        return cmd;
}


static command* is_variable(char* cmdline){
    
    command* cmd=NULL;
    unsigned short i=0;
    unsigned short j;
    char key[MAXLINE];
    char value[MAXLINE];
    while( cmdline[i++]=='='){
        if((cmdline[i]==SPACE)&&(cmdline[i]==ENTER)){
            return NULL;
        }
        // strncpy(key[MAXLINE],cmdline[0],i-1);
        j=i+1;
    }
    if(cmdline[j]==SPACE) return NULL;
    while(cmdline[j++]!=SPACE);
    // strncpy(value,cmdline[i],j-i);
    cmd=malloc(sizeof(command));
    cmd->f=VARIABLE;
    cmd->variable=malloc(sizeof(variable));
    cmd->variable->key=malloc(i);
    cmd->variable->value=malloc(j-i);
    strncpy(cmd->variable->key,cmdline[0],i-1);
    strncpy(cmd->variable->value,cmdline[i],j-i);

    return cmd;
}


// when exit free variablew
// static void allocate_variable(command* var){

//     if(num_variable==variable_list_size||variable_list_size==1){
//         variable_list_size=variable_list_size<<1;
//         variable_list=malloc(sizeof(variable*)*variable_list_size);
//     }
//     variable_list[num_variable++]=var;

//     return;
// }

void free_all_command(command** cmd){

}


static void find_arguments(command* cmd,char *cmdline,int *pipen){

}
