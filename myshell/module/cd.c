#include "cd.h"
void change_directory(char** argv){
    if(chdir(argv[1])<0){
        fprintf(stderr,"No Such Directory\n");
    }
    fflush(stdin); fflush(stdout);
}