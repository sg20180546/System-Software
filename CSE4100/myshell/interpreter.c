#include "interperter.h"
#include "type.h"
#include "myshell.h"
static void parser();

/*
FOREGROUND EXECUTING - suspend parent process, until forked child process send SIGCHILD

BACKGROUDN EXECUTING - parent do parent, child do child
*/



void interpreter(char* cmdline,char* bgbuffer){
    char* command[MAXARGS];
    unsigned short pipen;
    bool is_background;
    char* buf;
    strcpy(buf,cmdline);
    parser(buf,command,pipen,&is_background);
    
}

// find builtin name by binary search
// need to make builtin main function, excecutable object file
// builtin list-> in complile time? run time?
static void parser(char* cmdline,char** command,unsigned short* pipen,bool* is_background){
    unsigned int i=0;
    while(cmdline[i++]!=ENTER){
        while(cmdline[i++]!=SPACE);
        // how to check and save command efficeintly, not using strncmp? -> binary search
        // pipeline -> how to write other proccess file descriptor 0 of file table, whcih is stdin?
        // |-> because child copy same file table with parent, writing to 1 is can wathced in other process
    }
}


