#include "interperter.h"


// #include "myshell.h"


/*
FOREGROUND EXECUTING - suspend parent process, until forked child process send SIGCHILD

BACKGROUDN EXECUTING - parent do parent, child do child
*/
static status parser(char* cmdline,struct command** first_command,struct command** last_command,MODE* mode);
static status find_shell_command(char* cmdline,int* pos,struct command** cbuf);
static struct command* is_variable(char* cmdline,int* pos);
static status find_arguments(struct command** cmd,char *cmdline,unsigned int* pos);
static void free_command_list(struct command* cmd);
static void free_command(struct command* cmd);
unsigned int    running_background_jobs_front=0;
unsigned int    running_background_jobs_rear=0;
int    num_variable=0;
int    variable_list_size=1;
int    num_builtin_command=CMD_VARIABLE-1;



// HOW TO INSTALL SIGNAL HANDLER consistenly both back and fore?
void sigchild_handler_child(int sig){
    while(waitpid(-1,NULL,0)>0);
}

void sigchild_handler_parent(int sig){
    while(waitpid(-1,NULL,0)>0){

    }
}






void interpreter(char* cmdline){
    pid_t child_pid;
    pipe(fds);
    

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
    // a(first_command->redirectto->arguments[2]==NULL);

    a(first_command!=NULL);
    a(last_command!=NULL);
 
    
    if(first_command->f=FUNCTION&&mode==FOREGROUND){
        execute_function_command(first_command);
        first_command=first_command->redirectto;
    }

    if((child_pid=fork())==0){
        switch (status)
        {
        case OK:
            // if background job, wait at child
            if(mode==BACKGROUND){
                // make new process group id
                printf("[BG] %d : %s\n",child_pid,buf);
                signal(SIGCHLD,sigchild_handler_child);
                execute_commands(first_command);
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
        exit(45);
    }else{
        // if foreground job, wait at parent
        // pd("wating");
        if(mode==FOREGROUND){
            close(fds[READ_END]); close(fds[WRITE_END]);
            sigset_t mask;
            sigemptyset(&mask);
            sigsuspend(&mask);
        }else{


        }
    }

    free_command_list(first_command);
}

// find builtin name by binary search
// need to make builtin main function, excecutable object file
// builtin list-> in complile time? run time?
static status parser(char* cmdline,struct command** first_command,struct command** last_command,MODE* mode){
    status status;
    unsigned int pos=0;
    struct command* previous_command=NULL;

    while(cmdline[pos]!=ENTER){
        while(whitespace(cmdline[pos])) pos++;
        if(cmdline[pos]==ENTER) return status;
        struct command* cbuf;


        status=find_shell_command(cmdline,&pos,&cbuf);
        if(status!=OK) return status;

        if(!(*first_command)) *first_command=cbuf;
        (*last_command)=cbuf;
  
        if(previous_command){
          cbuf->redirectfrom=previous_command;
          previous_command->redirectto=cbuf;  
        }
        previous_command=cbuf;
        

        if( (cbuf->f==STATIC)||(cbuf->f=FUNCTION)){
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

            int c=cmdline[*pos]-command_list[mid].builtin->name[0];
            if(c==0){
              if(!strncmp(&cmdline[*pos],command_list[mid].builtin->name,strlen(command_list[mid].builtin->name))){
                  *cbuf=malloc(sizeof(struct command));
                  (*cbuf)->f=command_list[mid].f;
                  (*cbuf)->builtin=malloc(sizeof(static_command));
                  (*cbuf)->builtin->name=malloc(sizeof(command_list[mid].builtin->name));
                  strcpy((*cbuf)->builtin->name,command_list[mid].builtin->name);
                  *pos+=strlen((*cbuf)->builtin->name);
                  break;
              }else{
                  if( (cmdline[*pos+1]-command_list[mid].builtin->name[1])>0) low=mid+1;
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



static status find_arguments(struct command** cmd,char *cmdline,unsigned int* pos){
    status status=OK;
    (*cmd)->argc=0;
    (*cmd)->arguments=malloc(sizeof(char*)*MAXARGS);
    (*cmd)->arguments[(*cmd)->argc++]=malloc(strlen((*cmd)->builtin->name));
    strcpy((*cmd)->arguments[0],(*cmd)->builtin->name);
    while(1){


        if(cmdline[*pos]==PIPE||cmdline[*pos]==ENTER){
            break;
        }
        unsigned int i=*pos;
        while((!whitespace(cmdline[*pos])) ){
            (*pos)=(*pos)+1;
            if(cmdline[*pos]==10){
                break;
            }
        } 

        int size=(*pos)-i;
        (*cmd)->arguments[(*cmd)->argc]=(char*)malloc(size);
        strncpy((*cmd)->arguments[(*cmd)->argc++],&cmdline[i],size);
        while(!whitespace(cmdline[*pos])){
            if(cmdline[(*pos)]==ENTER) break; //10
        }
    }
    a((*cmd)->arguments[(*cmd)->argc]==NULL);
    return status;
}

static void free_command_list(struct command* cmd){
    struct commmad* next;
    for(;cmd;cmd=cmd->redirectto){
        free_command(cmd);
    }
}

static void free_command(struct command* cmd){
    int i;
    for(i=0;i<=cmd->argc;i++){
        free(cmd->arguments[0]);
    }
    free(cmd->arguments);
    free(cmd->builtin);
    free(cmd);
}