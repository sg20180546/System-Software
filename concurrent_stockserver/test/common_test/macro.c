#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
// void child(int sig){
//     printf("sigchildcome\n");
//     // while(waitpid(0,NULL,0)){
        
//     //     kill(getpid(),SIGCONT);
//     // }
// }

int main(){
    // signal(SIGCHLD,child);
    int i;
    sigset_t ss;
    pid_t pid;
    char client_n[10]="64";
    char total_q[30]="4194304";
    char t[10]="-t32";
// optimizde thread test
    // for(i=256;i>=1;i=i/2){
    //     sprintf(t,"-t%d",i);
    //     pid=fork();
    //     if(pid==0){
    //         char port[5];
    //         sprintf(port,"%d",1024+i);
    //         printf("\n\nThread n : %s port : %s\n",t,port);
    //         char* argv[]={"./benchmark",port,client_n,total_q,t,NULL};
    //         execv("./benchmark",argv);
    //         exit(0);
    //     }
    //     wait(0);
    // }

// optmize client n test in thread_Based
    // for(i=512;i>=1;i=i/2){
    //     sprintf(client_n,"%d",i);
    //     pid=fork();
    //     if(pid==0){
    //         char port[5];
    //         sprintf(port,"%d",1024+256+i);
    //         printf("\n\nclient_n : %s port : %s\n",client_n,port);
    //         char* argv[]={"./benchmark",port,client_n,total_q,t,NULL};
    //         execv("./benchmark",argv);
    //         exit(0);
    //     }
    //     wait(0);
    // }

// optmize client n test in event_driven
    // strcpy(t,"-e");
    // for(i=512;i>=1;i=i/2){
    //     sprintf(client_n,"%d",i);
    //     pid=fork();
    //     if(pid==0){
    //         char port[5];
    //         sprintf(port,"%d",1024+256+i);
    //         printf("\n\nclient_n : %s port : %s\n",client_n,port);
    //         char* argv[]={"./benchmark",port,client_n,total_q,t,NULL};
    //         execv("./benchmark",argv);
    //         exit(0);
    //     }
    //     wait(0);
    // }

// thread vs event
    
    int total_q_integer=1024;
    int j=0;
    for(i=total_q_integer;total_q_integer<=4194304;i=i*4){
        sprintf(total_q,"%d",i);
        printf("\033[0;32m::Total Query : %d::\033[0;0m\n",i);
        pid=fork();
        if(pid==0){
            char port[10];
            sprintf(port,"%d",1024+j);
            char t[]="-t32";
            printf("\n::Thread_based(t=32) port : %s\n",port);
            char* argv[]={"./benchmark",port,client_n,total_q,t,NULL};
            execv("./benchmark",argv);
            exit(0);
        }
        wait(0);

        pid=fork();
        if(pid==0){
            char port[10];
            sprintf(port,"%d",4096+j);
            char e[]="-e";
            printf("\n::Event_driven : %s port : %s\n\n\n\n",t,port);
            char* argv[]={"./benchmark",port,client_n,total_q,e,NULL};
            execv("./benchmark",argv);
            exit(0);
        }
        wait(0);
        j++;
    }

}