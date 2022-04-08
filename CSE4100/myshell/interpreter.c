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

int    num_variable=0;
int    variable_list_size=1;
int    num_builtin_command=CMD_VARIABLE-1;



void interpreter(char* cmdline){

    sigset_t mask;
    sigemptyset(&mask);
    getcwd(buf,MAXLINE);
    printf("%s>",buf);
    fgets(cmdline,MAXLINE,stdin);
    if(feof(stdin)) exit(0);

    if(jobs_done[0]) print_done_jobs();
    
    status status=NOCOMMANDERR;
    struct command* first_command,*last_command;
    first_command=last_command=NULL;

    MODE mode=FOREGROUND;

    strcpy(buf,cmdline);

    if(status==BUFFERING) status=parser(buf,&last_command,&last_command,&mode);
    else status=parser(buf,&first_command,&last_command,&mode);
        if(first_command&&first_command->f==FUNCTION&&mode==FOREGROUND){
            execute_function_command(first_command);
            
            
        struct command* buf=first_command->redirectto;
        free_command(first_command);
        first_command=buf;
            
        }
    pid_t pid=fork();
    if(pid&&mode==FOREGROUND) child_pid=pid;
    else if(pid&&mode==BACKGROUND){
        printf("[%d] : %s",jobs_rear,buf);
        insert_jobs(pid,buf,RUNNING);
    }


    if(pid==0){

        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        signal(SIGCHLD,sigchild_handler_child);
        switch (status)
        {
        case OK:
            // if background job, wait at child

            execute_commands(first_command,0);
            break;
        case BUFFERING:
            // goto l1;
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

        sigsuspend(&mask);
        if(mode==BACKGROUND){

        } 
        exit(0);
    }else{
        // if foreground job, wait at parent
        if(mode==FOREGROUND){
            reap_dead_jobs();
            sigsuspend(&mask);
        }else{
            

        }
        free_command_list(first_command);
    }
    
}
// https://github.com/sg20180546/CSAPP/blob/d06045832b0586638365cce91e4cd876868f5faf/CSE4100/myshell/interpreter.c
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

        if(!(*first_command)) (*first_command)=cbuf;
        (*last_command)=cbuf;
  
        if(previous_command!=NULL){
          cbuf->redirectfrom=previous_command;
          previous_command->redirectto=cbuf;  
        }
        previous_command=cbuf;

        if(cbuf->f!=VARIABLE){
            while(whitespace(cmdline[pos])) pos++;
            status=find_arguments(&cbuf,cmdline,&pos);
            if(status!=OK){
                return status;
            }
        }
        
      while(whitespace(cmdline[pos])) pos++;

        if(cmdline[pos]==AMPERSAND){
            *mode=BACKGROUND;
            pos++;
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
    }
    return status;
}

static status find_shell_command(char* cmdline,int* pos,struct command** cbuf){
        status status=OK;
        (*cbuf)=NULL;
        short low=0,high=num_builtin_command,mid;
        
        if(cmdline[*pos]=='.'){
           
            int i=*pos;
            
           
            while( !whitespace(cmdline[*pos])&& cmdline[*pos]!=ENTER&& cmdline[*pos]!=PIPE ){
                
                (*pos)++;
            }
            
            *cbuf=calloc(1,sizeof(struct command));
            (*cbuf)->f=RELATIVE;
            (*cbuf)->builtin=calloc(1,sizeof(builtin));
            (*cbuf)->builtin->name=calloc(1,(*pos)-i);
            strncpy((*cbuf)->builtin->name,&cmdline[i],(*pos)-i);
            
            return status;
        }

        while(low<=high){
            mid=(low+high)/2;

            int c=cmdline[*pos]-command_list[mid].builtin->name[0];
  
            if(c==0){
              if(!strncmp(&cmdline[*pos],command_list[mid].builtin->name,strlen(command_list[mid].builtin->name))){
                  *cbuf=calloc(1,sizeof(struct command));
                  (*cbuf)->f=command_list[mid].f;
                  (*cbuf)->builtin=calloc(1,sizeof(builtin));
                  if((*cbuf)->f==FUNCTION) (*cbuf)->builtin->fp=command_list[mid].builtin->fp;
                
                  
                  (*cbuf)->builtin->name=calloc(1,sizeof(command_list[mid].builtin->name));
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
        if((*cbuf)==NULL) status=NOCOMMANDERR;
       
        return status;
}


static struct command* is_variable(char* cmdline,int* pos){
    
    struct command* cmd=NULL;
    unsigned short i=*pos;
    unsigned short j;

    while( cmdline[*pos]!='='){
        if((whitespace(cmdline[*pos]))||(cmdline[*pos]==ENTER)){
            return NULL;
        }
        *pos++;
        // strncpy(key[MAXLINE],cmdline[0],i-1);
    }
    j=*pos++;
    if(whitespace(cmdline[*pos]||cmdline[*pos]==ENTER)) return NULL;
    while(!whitespace(cmdline[*pos])) *pos++;
        
    cmd=calloc(1,sizeof(struct command));
    cmd->f=VARIABLE;
    cmd->variable=calloc(1,sizeof(variable));
    cmd->variable->key=calloc(1,j-i);
    cmd->variable->value=calloc(1, (*pos)-j );
    strncpy(cmd->variable->key,(&cmdline[i]),j-1);
    strncpy(cmd->variable->value,&cmdline[j+2], *pos - j );
    return cmd;
}




static status find_arguments(struct command** cmd,char *cmdline,unsigned int* pos){
    status status=OK;
    int argc=0;
    (*cmd)->arguments=calloc(1,sizeof(char*)*MAXARGS);
    (*cmd)->arguments[argc++]=calloc(1,strlen((*cmd)->builtin->name));

    strcpy((*cmd)->arguments[0],(*cmd)->builtin->name);
    for(;argc<MAXARGS;argc++){

        if(cmdline[*pos]==AMPERSAND) return status;        
        unsigned int i=*pos;

        int size=0;
        if(cmdline[*pos]==DAPOSTROPHE){
            i++;
            while(cmdline[i]!=DAPOSTROPHE){
                if(cmdline[i]==PIPE||cmdline[i]==ENTER){
                    size=-1;
                    break;
                }
                i++;
            }
            if(size==-1){
                // only "
                (*cmd)->arguments[argc]=calloc(1,2);
                strcpy((*cmd)->arguments[argc],"\"");
                i=(*pos);
            }else{
                size=i-(*pos)-1;
                (*cmd)->arguments[argc]=calloc(1,size);
                strncpy((*cmd)->arguments[argc],&cmdline[(*pos)+1],size);
            }
            (*pos)=i+1;
        }else{

            while((!whitespace(cmdline[*pos])) ){
                if(cmdline[*pos]==ENTER||cmdline[*pos]==PIPE){
                    break;
                }
                (*pos)=(*pos)+1;
            } 

            size=(*pos)-i;
            if(size==0) break;
            (*cmd)->arguments[argc]=(char*)calloc(1,size);
            strncpy((*cmd)->arguments[argc],&cmdline[i],size);
            
        }
       while(whitespace(cmdline[*pos])){
           if(cmdline[(*pos)]==ENTER) break; //10
                (*pos)++;
        }
    }
    (*cmd)->arguments[argc]=NULL;
    // printf("%s  len : %ld \n",(*cmd)->builtin->name,strlen((*cmd)->arguments[1]));
    (*cmd)->argc=argc;

    return status;
}

static void free_command_list(struct command* cmd){
    if(cmd==NULL){
        return;
    } 
    free_command_list(cmd->redirectto);
    free_command(cmd);
}

static void free_command(struct command* cmd){
    int i;
    if(!cmd) return;
    for(i=0;i<(cmd->argc);i++){
        // if(!cmd->arguments[i]) continue;
        free(cmd->arguments[i]);
    }
    // a(cmd->arguments!=NULL);
    free(cmd->arguments);
    // a(cmd->builtin!=NULL);
    free(cmd->builtin);
    // a(cmd!=NULL);
    free(cmd);
}