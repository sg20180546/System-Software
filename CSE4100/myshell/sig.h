#ifndef _SIG_H_
#define _SIG_H_

#include "common.h"
#include "type.h"
#include "interperter.h"

#define SEND_CONTINUE(x) kill(x,SIGCONT)
#define SEND_INT(x) kill(x,SIGINT)
#define SEND_TSTP(x) kill(x,SIGTSTP)
#define SEND_KILL(x) kill(x,SIGKILL)
#define SEND_USR1(x) kill(x,SIGUSR1)

int foreground_job;

void sigchild_handler_child(int sig);

void sigchild_handler(int sig);

void sigint_handler(int sig);

void sigtstp_handler(int sig);



void sigttou_handler(int sig);

void sigttin_handler(int sig);

void sigpipe_handler(int sig);


#endif

