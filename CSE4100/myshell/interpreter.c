#include "interperter.h"
#include "type.h"
#include "builtin.h"
#include "common.h"
#include "execute.h"


// #include "myshell.h"


/*
FOREGROUND EXECUTING - suspend parent process, until forked child process send SIGCHILD

BACKGROUDN EXECUTING - parent do parent, child do child
*/
static status parser(char* cmdline,command* first_command,command* last_command,bool* is_background);
static status find_shell_command(char* cmdline,int* pos,command* cbuf);
static command* is_variable(char* cmdline,int* pos);
static status find_arguments(command* cmd,char *cmdline,unsigned int* pos);

command command_list[CMD_VARIABLE]={
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"cat"}},
        {0,NULL,STATIC,NULL,NULL,0,&(static_command){"cd"}},
            {0,NULL,STATIC,NULL,NULL,0,&(static_command){"clear"}},
            {0,NULL,STATIC,NULL,NULL,0,&(static_command){"echo"}},
            {0,NULL,STATIC,NULL,NULL,0,&(static_command){"exit"}},
            {0,NULL,CUSTOM,NULL,NULL,0,&(static_command){"jobs"}},
            {0,NULL,STATIC,NULL,NULL,0,&(static_command){"ls"}},
            {0,NULL,STATIC,NULL,NULL,0,&(static_command){"mkdir"}},
            {0,NULL,STATIC,NULL,NULL,0,&(static_command){"pwd"}},
            {0,NULL,STATIC,NULL,NULL,0,&(static_command){"rmdir"}},
            {0,NULL,STATIC,NULL,NULL,0,&(static_command){"touch"}},
            {0,NULL,STATIC,NULL,NULL,0,&(static_command){"variable"}} };

unsigned int    running_background_jobs_front=0;
unsigned int    running_background_jobs_rear=0;
int    num_variable=0;
int    variable_list_size=1;
int    num_builtin_command=CMD_VARIABLE-1;


void sigchild_handler(int sig){

}


void interpreter(char* cmdline){

    signal(SIGCHLD,sigchild_handler);

    l1:
    write(1,">",1);
    fgets(cmdline,MAXLINE,stdin);
    if(feof(stdin)) exit(0);

    status status=NOCOMMANDERR;
    command* first_command,*last_command;
    unsigned short pipen=0;
    bool is_background;
    char buf[MAXLINE];
    strcpy(buf,cmdline);
    
    if(status==BUFFERING) status=parser(buf,last_command,last_command,&is_background);
    else status=parser(buf,first_command,last_command,&is_background);

    pid_t pid;
    if((pid=fork())==0){
        switch (status)
        {
        case OK:
            // if background job, wait at child
            if(is_background.v){
                sigset_t mask;
                sigaddset(&mask,SIGCHLD);
                sigprocmask(SIG_BLOCK,&mask,NULL);
                execute_commands(first_command);
                while(waitpid(-1,NULL,0)>0);
                exit(0);
            }
            execute_commands(first_command);
            break;
        case BUFFERING:
            goto l1;
            break;
        case SYNTAXERR:
            break;
        case NOCOMMANDERR:
            break;
        default:
            break;
        }
    }
    // if foreground job, wait at parent
    if(pid!=0&&!is_background.v){
        while(waitpid(pid,NULL,0)>0);
    }else{
        running_background_jobs[running_background_jobs_rear]=malloc(strlen(buf));
        strcpy(running_background_jobs[running_background_jobs_rear],buf);
        running_background_jobs_rear=(running_background_jobs_rear+1)%MAXARGS;
    }
         
}

// find builtin name by binary search
// need to make builtin main function, excecutable object file
// builtin list-> in complile time? run time?
static status parser(char* cmdline,command* first_command,command* last_command,bool* is_background){
    status status=OK;
    unsigned int pos=0;
    command* previous_command=first_command;
    while(cmdline[pos]!=ENTER){
        while(whitespace(cmdline[pos++]));
        
        if(cmdline[pos]==ENTER) return status;

        command* cbuf=NULL;
        status=find_shell_command(cmdline,&pos,cbuf);
        if(status!=OK) return status;
        if(!first_command) first_command=cbuf;

        cbuf->redirectfrom=previous_command;
        if(previous_command) previous_command->redirectto=cbuf;
        previous_command=cbuf;
        last_command=cbuf;
        

        if(cbuf->f==STATIC){
            while(whitespace(cmdline[pos++]));
            status=find_arguments(cbuf,cmdline,&pos);
            if(status!=OK) return status;
        }




        while(whitespace(cmdline[pos++]));
        
        if(cmdline[pos]==BACKGROUND){
            is_background->v=1;
            while(whitespace(cmdline[pos++]));
            if(cmdline[pos]!=ENTER){
                status=SYNTAXERR;
            }
            return status;
        }
        else if(cmdline[pos++]==PIPE){
            status=BUFFERING;
        }


        // while(cmdline[i++]!=SPACE)

        // if(cmdline[i]==PIPE) *pipen++;




        // how to check and save command efficeintly, not using strncmp? -> binary search
        // pipeline -> how to write other proccess file descriptor 0 of file table, whcih is stdin?
        // |-> because child copy same file table with parent, writing to 1 is can wathced in other process
        //binarysearch

        // if not found , found abc=test
        

        // if not unfound command

    }
    return status;
}

static status find_shell_command(char* cmdline,int* pos,command* cbuf){
        status status=OK;
        unsigned short low=0,high=num_builtin_command,mid;
        // command* cmd=NULL;
        
        while(low<=high){
            mid=(low+high)/2;

            int c=cmdline[*pos]-command_list[mid].static_cmd->name[0];

            if(c==0){
              if(!strncmp(cmdline,command_list[mid].static_cmd->name,sizeof(command_list[mid].static_cmd->name))){
                  cbuf=malloc(sizeof(command));
                  cbuf->f=STATIC;
                  cbuf->static_cmd=malloc(sizeof(static_command));
                  cbuf->static_cmd->name=malloc(sizeof(command_list[mid].static_cmd->name));
                  strcpy(cbuf->static_cmd->name,command_list[mid].static_cmd->name);
                  *pos+=sizeof(cbuf->static_cmd->name);
              }else{
                  if( (cmdline[*pos+1]-command_list[mid].static_cmd->name[1])>0) low=mid+1;
                  else high=mid-1;
              }
            } 
            else if(c>0) low=mid+1;
            else high=mid-1;

        }
        if(cbuf==NULL){
            cbuf=is_variable(cmdline,pos);
        }
        if(cbuf==NULL) status=NOCOMMANDERR;
        
        return status;
}


static command* is_variable(char* cmdline,int* pos){
    
    command* cmd=NULL;
    unsigned short i=*pos;
    unsigned short j;

    while( cmdline[++*pos]!='='){
        if((cmdline[i]==SPACE)&&(cmdline[i]==ENTER)){
            return NULL;
        }
        // strncpy(key[MAXLINE],cmdline[0],i-1);
        j=*(pos-1);
    }
    if(cmdline[*pos++]==SPACE) return NULL;
    while(!whitespace(cmdline[*pos++]));
        
    // strncpy(value,cmdline[i],j-i);
    cmd=malloc(sizeof(command));
    cmd->f=VARIABLE;
    cmd->variable=malloc(sizeof(variable));
    cmd->variable->key=malloc(j-i);
    cmd->variable->value=malloc( (*pos)-j );
    strncpy(cmd->variable->key,(&cmdline[i]),j-1);
    strncpy(cmd->variable->value,&cmdline[j+2], *pos - j );
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


static status find_arguments(command* cmd,char *cmdline,unsigned int* pos){
    status status;
    cmd->argc=0;
    cmd->arguments=malloc(sizeof(char*)*MAXARGS);
    cmd->arguments[cmd->argc++]=malloc(sizeof(cmd->static_cmd->name));
    strcpy(cmd->arguments[0],cmd->static_cmd->name);
    while(1){
        if(cmdline[*pos]==PIPE||cmdline[*pos]==ENTER) break;
        unsigned int i=*pos;
        while(1){
            if(cmdline[*pos]==PIPE||cmdline[*pos]==ENTER) break;
            else if(cmdline[*pos]==SPACE) break;
            *pos++;
        }
        unsigned int size=*pos-i;
        cmd->arguments[cmd->argc++]=malloc(sizeof(size));
        strncpy(cmd->arguments[cmd->argc],&cmdline[i],size);
        
        // else{

            // cmd->arguments[argc++]=malloc(*pos -i);
            // strncpy(cmd->arguments[argc],cmdline[i],*pos-i-1);
        // }
        // *pos++;
    }
    cmd->arguments[cmd->argc]=malloc(1);
    cmd->arguments[cmd->argc]=NULL;
    return status;
}
