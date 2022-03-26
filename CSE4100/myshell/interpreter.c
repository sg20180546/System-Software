#include "interperter.h"
#include "type.h"
#include "builtin.h"
#include "common.h"
// #include "myshell.h"
static void parser();

/*
FOREGROUND EXECUTING - suspend parent process, until forked child process send SIGCHILD

BACKGROUDN EXECUTING - parent do parent, child do child
*/



void interpreter(char* cmdline,char* bgbuffer){
    status err;
    command command[MAXARGS];
    unsigned short pipen=0;
    bool is_background;
    char* buf;
    strcpy(buf,cmdline);
    parser(buf,command,pipen,&is_background,&err);
 
        switch (err)
        {
        case OK:
            if(is_background.v){
                
            }
            // wait child process
            wait(NULL);
            /* code */
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
static void parser(char* cmdline,command* _command,unsigned short* pipen,bool* is_background,status* err){
    unsigned int i=0;
    while(cmdline[i++]!=ENTER){
        while(cmdline[i++]!=SPACE);

        is_variable(&cmdline[i]);
        command* cbuf=find_shell_command(cmdline);
        if(cbuf==NULL){
            *err=NOCOMMANDERR;
            return;
        }
        _command[*pipen]=*cbuf;
        // find_arguments(&cmdline[i]);

        while(cmdline[i++]!=SPACE);

        if(cmdline[i]==PIPE) *pipen++;

        // how to check and save command efficeintly, not using strncmp? -> binary search
        // pipeline -> how to write other proccess file descriptor 0 of file table, whcih is stdin?
        // |-> because child copy same file table with parent, writing to 1 is can wathced in other process

        //binarysearch

        // if not found , found abc=test
        

        // if not unfound command

    }
}

static command* find_shell_command(char* cmdline){
        unsigned short low=0,high=num_builtin_command,mid;
        while(low<=high){
            mid=(low+high)/2;

            int c=cmdline[0]-command_list[mid].name[0];

            if(c==0){
              if(!strncmp(cmdline,command_list[mid].name,sizeof(command_list[mid].name))){
                  return &command_list[mid];
              }  
            } 
            else if(c>0) low=mid+1;
            else high=mid-1;

        }
        // char* pos_variable=cmdline;
        // unsigned short i=1;
        // while( (cmdline[i]!=ENTER)&&(cmdline[i]=SPACE) ){
        //     if(cmdline[i]=='='){
        //         unsigned short j=i;
                
        //         // strcpy(command_list[CMD_VARIABLE].argue,)

                
        //     }
        //     i++;
        // }

        return NULL;
}


static void is_variable(char* cmdline){
    unsigned short i=0;
    unsigned short j;
    char key[MAXLINE];
    char value[MAXLINE];
    while( cmdline[i++]=='='){
        if((cmdline[i]==SPACE)&&(cmdline[i]==ENTER)){
            return;
        }
        strncpy(key[MAXLINE],cmdline[0],i-1);
        j=i+1;
    }
    if(cmdline[j]==SPACE) return;
    while(cmdline[j++]!=SPACE);
    strncpy(value,cmdline[i],j-i);

    if(find_shell_command(key)) return;

    allocate_variable(key,value);
}


// when exit free variablew
static void allocate_variable(char* key,char* value){

    if(num_variable==variable_list_size||variable_list_size==1){
        variable_list_size=variable_list_size<<1;
        variable_list=malloc(sizeof(variable*)*variable_list_size);
    }
    variable_list[num_variable]=malloc(sizeof(variable));
    strcpy(variable_list[num_variable]->key,key);
    strcpy(variable_list[num_variable]->value,value);

    return;
}

void free_all_variable(){

}


static void find_arguments(char *cmdline){

}


