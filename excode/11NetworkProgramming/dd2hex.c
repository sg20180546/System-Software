#include "../csapp.h"

int main(int argc,char**argv){
    struct in_addr inaddr;
    int rc;
    if(argc!=2) exit(0);
    // 0.0.0.0->0x
    rc=inet_pton(AF_INET,argv[1],&inaddr);
    if(rc==0) app_error("inet_pton error");
    else if(rc<0) unix_error("inet_pton_Error");
    printf("0x%x\n",ntohl(inaddr.s_addr));
    exit(0);
}