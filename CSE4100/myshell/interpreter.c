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



void interpreter(char* cmdline){
    
    write(1,">",1);
    fgets(cmdline,MAXLINE,stdin);
    if(feof(stdin)) exit(0);

    status status=NOCOMMANDERR;
    struct command* first_command,*last_command;
    first_command=last_command=NULL;

    MODE mode=FOREGROUND;
    char buf[MAXLINE];
    strcpy(buf,cmdline);

    if(status==BUFFERING) status=parser(buf,&last_command,&last_command,&mode);
    else status=parser(buf,&first_command,&last_command,&mode);
        if(first_command&&first_command->f==FUNCTION&&mode==FOREGROUND){
            execute_function_command(first_command);
            
            
        struct command* buf=first_command->redirectto;
        free_command(first_command);
        first_command=buf;
            
            
        }
    // pd(first_command->redirectto->redirectto->builtin->name);
    if((child_pid=fork())==0){
        pipe(fds);
        pid_t pid=getpid();
        // setpgrp();
        signal(SIGCHLD,SIG_DFL);
        signal(SIGINT,SIG_DFL);
        signal(SIGSTOP,SIG_DFL);
        // signal(SIGCHLD,sigchild_handler_child);
        switch (status)
        {
        case OK:
            // if background job, wait at child
            if(mode==BACKGROUND){
                // make new process group id
                printf("[BG] %d : %s\n",child_pid,buf);
                
                insert_jobs(pid,buf,0);
                // signal(SIGCHLD, SIG_DFL);
                // signal(SIGCHLD,sigchild_handler_child); //wake up to 94
                execute_commands(first_command);
                exit(0);
            }

            execute_commands(first_command);
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
        close(fds[READ_END]); close(fds[WRITE_END]);
        exit(0);
    }else{
        // if foreground job, wait at parent
        if(mode==FOREGROUND){
            reap_dead_jobs();
            sigset_t mask;
            sigemptyset(&mask);
            sigsuspend(&mask);
        }else{


        }
        free_command_list(first_command);
        // a(first_command==NULL);
    }
    
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

        if(!(*first_command)) (*first_command)=cbuf;
        (*last_command)=cbuf;
  
        if(previous_command!=NULL){
          cbuf->redirectfrom=previous_command;
          previous_command->redirectto=cbuf;  
        }
        previous_command=cbuf;
        // pd(cbuf->builtin->name);
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
            // printf("low : %d high :%d\n",low,high);
            int c=cmdline[*pos]-command_list[mid].builtin->name[0];
            // printf("%s\n",command_list[mid].builtin->name);
            if(c==0){
              if(!strncmp(&cmdline[*pos],command_list[mid].builtin->name,strlen(command_list[mid].builtin->name))){
                //   pd("same");
                  *cbuf=calloc(1,sizeof(struct command));
                  (*cbuf)->f=command_list[mid].f;
                  (*cbuf)->builtin=calloc(1,sizeof(builtin));
                  if((*cbuf)->f==FUNCTION) (*cbuf)->builtin->fp=command_list[mid].builtin->fp;
                
                  
                  (*cbuf)->builtin->name=calloc(1,sizeof(command_list[mid].builtin->name));
                  strcpy((*cbuf)->builtin->name,command_list[mid].builtin->name);
                  *pos+=strlen((*cbuf)->builtin->name);
                  break;
              }else{
                //   pd("here???");
                  if( (cmdline[*pos+1]-command_list[mid].builtin->name[1])>0) low=mid+1;
                  else high=mid-1;
              }
            } 
            else if(c>0) low=mid+1;
            else high=mid-1;

        }
        // pd("hi");
        // a((*cbuf)==NULL);
        if((*cbuf)==NULL) status=NOCOMMANDERR;
       
        return status;
}


static struct command* is_variable(char* cmdline,int* pos){
    
    struct command* cmd=NULL;
    unsigned short i=*pos;
    unsigned short j;

    // pd("here2?");
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
        
    // strncpy(value,cmdline[i],j-i);
    cmd=calloc(1,sizeof(struct command));
    cmd->f=VARIABLE;
    cmd->variable=calloc(1,sizeof(variable));
    cmd->variable->key=calloc(1,j-i);
    cmd->variable->value=calloc(1, (*pos)-j );
    strncpy(cmd->variable->key,(&cmdline[i]),j-1);
    strncpy(cmd->variable->value,&cmdline[j+2], *pos - j );
    return cmd;
}


// when exit free variablew
// static void allocate_variable(command* var){

//     if(num_variable==variable_list_size||variable_list_size==1){
//         variable_list_size=variable_list_size<<1;
//         variable_list=calloc(1,sizeof(variable*)*variable_list_size);
//     }
//     variable_list[num_variable++]=var;

//     return;
// }



static status find_arguments(struct command** cmd,char *cmdline,unsigned int* pos){
    status status=OK;
    int argc=0;
    (*cmd)->arguments=calloc(1,sizeof(char*)*MAXARGS);
    (*cmd)->arguments[argc++]=calloc(1,strlen((*cmd)->builtin->name));

    strcpy((*cmd)->arguments[0],(*cmd)->builtin->name);
    for(;argc<MAXARGS;argc++){


        
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

            while(!whitespace(cmdline[*pos])){
                if(cmdline[(*pos)]==ENTER) break; //10
                (*pos)++;
            }
        }
    }
    (*cmd)->argc=argc;
    a((*cmd)->arguments[(*cmd)->argc]==NULL);
    return status;
}

static void free_command_list(struct command* cmd){
    if(cmd==NULL){
        // pd("its null");
        return;
    } 
    free_command_list(cmd->redirectto);
    free_command(cmd);
}

static void free_command(struct command* cmd){
    int i;
    if(!cmd) return;
    // pd("asdfew");
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