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



void interpreter(char* cmdline){


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
    
    if(status==BUFFERING) parser(buf,last_command,last_command,&is_background,&status);
    else parser(buf,first_command,last_command,&is_background,&status);
    
    pid_t pid;
    if((pid=fork())==0){
        switch (status)
        {
        case OK:
            execute_commands(first_command);
            // if background job, wait at child
            if(is_background.v) while(waitpid(0,NULL,NULL)>0);
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
    }
         
}

// find builtin name by binary search
// need to make builtin main function, excecutable object file
// builtin list-> in complile time? run time?
static void parser(char* cmdline,command* first_command,command* last_command,bool* is_background,status* status){
    unsigned int pos=0;
    command* previous_command=first_command;
    while(cmdline[pos]!=ENTER){

        while( (cmdline[pos++]==SPACE));

        command* cbuf=NULL;
        *status=find_shell_command(cmdline,&pos,cbuf,status);
        if(*status!=OK) return;
        if(!first_command) first_command=cbuf;

        cbuf->redirectfrom=previous_command;
        if(previous_command) previous_command->redirectto=cbuf;
        previous_command=cbuf;
        last_command=cbuf;
        

        while(cmdline[pos++]==SPACE);
        *status=find_arguments(cbuf,cmdline,&pos,status);
        if(status!=OK) return;

        while(cmdline[pos++]==SPACE);

        if(cmdline[pos]==BACKGROUND){
            is_background->v=1;
            while(cmdline[pos++]==SPACE);
            if(cmdline[pos]!=ENTER){
                status=SYNTAXERR;
            }
            return;
        }
        else if(cmdline[pos]==PIPE){
            *status=BUFFERING;
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
}

static status* find_shell_command(char* cmdline,int* pos,command* cbuf,status* status){
        unsigned short low=0,high=num_builtin_command,mid;
        // command* cmd=NULL;

        while(low<=high){
            mid=(low+high)/2;

            int c=cmdline[0]-command_list[mid].static_cmd->name[0];

            if(c==0){
              if(!strncmp(cmdline,command_list[mid].static_cmd->name,sizeof(command_list[mid].static_cmd->name))){
                  cbuf=malloc(sizeof(command));
                  cbuf->f=STATIC;
                  cbuf->static_cmd=malloc(sizeof(static_command));
                  cbuf->static_cmd->name=malloc(sizeof(command_list[mid].static_cmd->name));
                  strcpy(cbuf->static_cmd->name,command_list[mid].static_cmd->name);
                  *pos+=sizeof(cbuf->static_cmd->name);
                  *status=OK;
              }  
            } 
            else if(c>0) low=mid+1;
            else high=mid-1;

        }
        if(cbuf==NULL){
            cbuf=is_variable(cmdline,pos);
        }
        if(cbuf==NULL) *status=NOCOMMANDERR;
        
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
    while(cmdline[*pos++]!=SPACE);

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


static status* find_arguments(command* cmd,char *cmdline,int* pos,status* status){
    if(cmdline[*pos++]!='-'){
        status=SYNTAXERR;
        return;
    }
    while((cmdline[*pos]!=ENTER)){

    }
}
