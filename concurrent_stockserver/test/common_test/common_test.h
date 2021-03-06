#ifndef COMMON_TEST_H_
#define COMMON_TEST_H_
#include <assert.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>

#define MAXNODE 100
#define SIGENDCLOCK SIGUSR1
#define BENCHMARK_CLIENT_ELF_PATH "../../client/multiclient_benchmark"
#define CLIENT_ELF_PATH "../../client/multiclient"
#define E_SERVER_PATH "../../event_driven"
#define T_SERVER_PATH "../../thread_based"


#define E_SERVER_ELF_PATH "../../event_driven/stockserver"
#define T_SERVER_ELF_PATH "../../thread_based/stockserver"
const char* phase;
struct timeval start,end;
double time_taken;

pid_t pid_server,pid_client;

#define TICK(X) clock_t X = clock()
#define TOCK(X) printf("time %s: %g sec.\n", (#X), (double)(clock() - (X)) / CLOCKS_PER_SEC)

#define CheckConditon(cond) \
    if(!(cond)){ \
        fprintf(stderr,"%s:%d: %s: %s\n",__FILE__,__LINE__,phase,#cond);\
        abort();    \
    }   
#define StartPhase(name) \
    fprintf(stderr,"=== Test %s\n",name);\
    gettimeofday(&start,NULL); \
    phase=name;

        


#endif