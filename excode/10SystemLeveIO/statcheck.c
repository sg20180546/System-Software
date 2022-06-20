#include "../csapp.h"
#include <stdlib.h>
int main(int argc,char**argv){
    gmmap_threshold();
    struct stat stat;
    char* type,*readok;
    int fd=open("dir",O_RDONLY,0);
    Fstat(fd,&stat);
    // Stat(argv[1],&stat);
    if(S_ISREG(stat.st_mode)) type="regular";
    else if(S_ISDIR(stat.st_mode)) type="directory";
    else type="other";
    if((stat.st_mode&S_IRUSR)) readok="yes";
    else readok="no";
    printf("type: %s,read %s link: %ld\n",type,readok,stat.st_nlink);
    exit(0);
}