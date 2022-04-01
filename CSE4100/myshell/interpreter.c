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
static status parser(char* cmdline,struct command** first_command,struct command** last_command,MODE* mode);
static status find_shell_command(char* cmdline,int* pos,struct command** cbuf);
static struct command* is_variable(char* cmdline,int* pos);
static status find_arguments(struct command** cmd,char *cmdline,unsigned int* pos);

struct command command_list[CMD_VARIABLE]={
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"cat"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"cd"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"clear"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"echo"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"exit"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"grep"}},
    {0,NULL,CUSTOM,NULL,NULL,0,&(static_command){"jobs"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"ls"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"mkdir"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"pwd"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"rmdir"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"sleep"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"touch"}},
    {0,NULL,STATIC,NULL,NULL,0,&(static_command){"variable"}} };

unsigned int    running_background_jobs_front=0;
unsigned int    running_background_jobs_rear=0;
int    num_variable=0;
int    variable_list_size=1;
int    num_builtin_command=CMD_VARIABLE-1;


void sigchild_handler(int sig){

}

  int debug=0;
void interpreter(char* cmdline){
    pipe(fds);
    signal(SIGCHLD,sigchild_handler);

    l1:
    write(1,">",1);
    fgets(cmdline,MAXLINE,stdin);
    if(feof(stdin)) exit(0);

    status status=NOCOMMANDERR;
    struct command* first_command,*last_command;
    first_command=last_command=NULL;
    a(first_command==NULL);
    a(last_command==NULL);


    MODE mode=FOREGROUND;
    char buf[MAXLINE];
    strcpy(buf,cmdline);

    if(status==BUFFERING) status=parser(buf,&last_command,&last_command,&mode);
    else status=parser(buf,&first_command,&last_command,&mode);


    a(first_command!=NULL);
    a(last_command!=NULL);
    pid_t pid;
    if((pid=fork())==0){
        switch (status)
        {
        case OK:
            // if background job, wait at child
            if(mode==BACKGROUND){
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
            write(1,"Syntax Error\n",14);
            break;
        case NOCOMMANDERR:
            write(1,"No Command Error\n",18);
            break;
        default:
            break;
        }
    }
    // if foreground job, wait at parent
    // pd("wating");
    if(pid!=0&&mode==FOREGROUND){
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
static status parser(char* cmdline,struct command** first_command,struct command** last_command,MODE* mode){
    status status;
    unsigned int pos=0;
    struct command* previous_command;

    while(cmdline[pos]!=ENTER){
        while(whitespace(cmdline[pos])) pos++;
        debug++;
        if(cmdline[pos]==ENTER) return status;
        struct command* cbuf;


        status=find_shell_command(cmdline,&pos,&cbuf);
        // pd(cbuf->static_cmd->name);
        if(status!=OK) return status;

        if(!(*first_command)) *first_command=cbuf;
        (*last_command)=cbuf;

        if(previous_command){
          cbuf->redirectfrom=previous_command;
          previous_command->redirectto=cbuf;  
        }
        previous_command=cbuf;
        

        if(cbuf->f==STATIC){
            while(whitespace(cmdline[pos])) pos++;
            status=find_arguments(&cbuf,cmdline,&pos);
            if(status!=OK){
                return status;
            }
        }

        // pd("hello");

        while(whitespace(cmdline[pos])) pos++;

        if(cmdline[pos]==AMPERSAND){
            *mode=BACKGROUND;
            while(whitespace(cmdline[pos])) pos++;
            if(cmdline[pos]!=ENTER){
                status=SYNTAXERR;
            }
            return status;
        }
        else if(cmdline[pos]==PIPE){
            status=BUFFERING;
            pos++;
        }
        if(debug==2) {
            // printf("%c %d\n",cmdline[pos], cmdline[pos]);
            a(cmdline[pos]==ENTER);
        }
        // pd(cbuf->arguments[0]);
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

static status find_shell_command(char* cmdline,int* pos,struct command** cbuf){
        status status=OK;
        unsigned short low=0,high=num_builtin_command,mid;

        while(low<=high){
            mid=(low+high)/2;

            int c=cmdline[*pos]-command_list[mid].static_cmd->name[0];
            if(c==0){
              if(!strncmp(&cmdline[*pos],command_list[mid].static_cmd->name,strlen(command_list[mid].static_cmd->name))){
                  *cbuf=malloc(sizeof(struct command));
                  (*cbuf)->f=command_list[mid].f;
                  (*cbuf)->static_cmd=malloc(sizeof(static_command));
                  (*cbuf)->static_cmd->name=malloc(sizeof(command_list[mid].static_cmd->name));
                  strcpy((*cbuf)->static_cmd->name,command_list[mid].static_cmd->name);
                  *pos+=strlen((*cbuf)->static_cmd->name);
                  break;
              }else{
                  if( (cmdline[*pos+1]-command_list[mid].static_cmd->name[1])>0) low=mid+1;
                  else high=mid-1;
              }
            } 
            else if(c>0) low=mid+1;
            else high=mid-1;

        }
        if(cbuf==NULL){
            *cbuf=is_variable(cmdline,pos);
        }
        if(cbuf==NULL) status=NOCOMMANDERR;
        return status;
}


static struct command* is_variable(char* cmdline,int* pos){
    
    struct command* cmd=NULL;
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
    cmd=malloc(sizeof(struct command));
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

void free_all_command(struct command** cmd){

}


static status find_arguments(struct command** cmd,char *cmdline,unsigned int* pos){
    status status=OK;
    (*cmd)->argc=0;
    (*cmd)->arguments=malloc(sizeof(char*)*MAXARGS);
    (*cmd)->arguments[(*cmd)->argc++]=malloc(strlen((*cmd)->static_cmd->name));
    strcpy((*cmd)->arguments[0],(*cmd)->static_cmd->name);
    while(1){

        // pd("loop");
        if(cmdline[*pos]==PIPE||cmdline[*pos]==ENTER){
            break;
        }
        unsigned int i=*pos;
        // pd("hello 222");
        // printf("char : %d ",cmdline[*pos]);
        while((!whitespace(cmdline[*pos])) ){
            (*pos)=(*pos)+1;
            if(cmdline[*pos]==10){
                break;
            }
        } 

        int size=(*pos)-i;
        (*cmd)->arguments[(*cmd)->argc]=(char*)malloc(size);
        strncpy((*cmd)->arguments[(*cmd)->argc++],&cmdline[i],size);

        // pd((*cmd)->arguments[1]);
        while(!whitespace(cmdline[*pos])){
            if(cmdline[(*pos)]==10) break;
        }
    }
    a((*cmd)->arguments[(*cmd)->argc]==NULL);
    // (*cmd)->arguments[(*cmd)->argc]=malloc(sizeof(NULL));
    // (*cmd)->arguments[(*cmd)->argc]=NULL;
    return status;
}
