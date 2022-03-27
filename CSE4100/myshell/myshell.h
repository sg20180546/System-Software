
#ifndef _MYSHELL_H_

#include "interperter.h"
#include "type.h"
#include "common.h"
#include "builtin.h"


char cmdline[MAXLINE];
char background_buffer[MAXLINE];

int builtin_command(char** argv);

void SIGCHILDhander();
// void cd();
// void ls(char* buf,char** options);
// void mkdir();
// void rmdir();
// void touch();
// void cat();
// void echo(char* buf,char** options,char* string);
// void variable();
// void exit();

void clear();
void pwd();
#endif