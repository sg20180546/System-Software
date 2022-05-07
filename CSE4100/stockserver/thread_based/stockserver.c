#include "stockserver.h"

int main(int argc,char** argv){

    if(argc!=2) exit(0);
    listenfd=open_listenfd(argv[1]);

    

}