#ifndef _SIG_H_
#define _SIG_H_

#include "common.h"
#include "type.h"

void sigchild_handler_child(int sig);

void sigchild_handler_parent(int sig);


void sigint_handler(int sig);

void sigtstp_handler(int sig);


#endif

