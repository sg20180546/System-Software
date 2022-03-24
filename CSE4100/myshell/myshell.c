#include "myshell.h"

// [1] 20878 background [1]+  Done
int main(){


    

    while(1){
        // if there is terminated background process, result
        write(1,">",1);
        fgets(cmdline,MAXLINE,stdin);
        if(feof(stdin)) exit(0);
        interpreter(cmdline,background_buffer);
        // if there is terminated background process, [1]+ Done\n with result
    }
}

void eval(char *cmdline){
    // char* argv[MAXARGS];
    char* environ[MAXARGS];
    // char buf[MAXLINE];
    unsigned short pipen;
    unsigned short i;
    pid_t pid;
    bool isBackGround;
    strcpy(buf,cmdline);

// parseline : get n of pipeline , get all command, get all environ to each command
// if pipeline exists, give buf to next command
    parseline(buf,argv,environ,&pipen,&isBackGround);
    if(argv[0]==NULL) return;

    for(i=0;i<pipen;i++){

    }
// while(pipeline n) 




    if(!builtin_command(argv)){
        if((pid=fork())==0){
            // execve : compile all executable object file ?
            if(execve(argv[0],argv,environ)<0){
                printf("%s: Command is not Found.\n",argv[0]);
                exit(0);
            }
        }
        // if(!bg){
        //     int status;
        //     if(waitpid(pid,&status,0)<0) fprintf(stderr,"waitfg: waitpid error");
        // }else{
        //     printf("%d %s",pid,cmdline);
        // }
        return;
    }

}

int builtin_command(char ** argv){
    // echo $abc abc="wdfweq" wc
    if(!strcmp(argv[0],"quit")) exit(0);
    if(!strcmp(argv[0],"&")) return 1;
    return 0;
}

void parseline(char* buf,char** argv,char** environ,unsigned short* pipen,bool* isBG){
    char* delim;
    int argc;
    int bg;
    
    buf[strlen(buf)-1]=' ';
    while(*buf&&(*buf)==' ') buf++;
    argc=0;
    while((delim=strchr(buf,' '))){
        argv[argc++]=buf;
        *delim='\0';
        buf=delim+1;
        while(*buf&&(*buf==' ')){
            buf++;
        }
    }
    argv[argc]=NULL;
    if(argc==0) return 1;

    if((bg=(*argv[argc-1]=='&')!=0)) argv[--argc]=NULL;
    return bg;
}