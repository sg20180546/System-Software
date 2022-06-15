#include "../csapp.h"
int main(int argc,char**argv){
    struct in_addr inaddr;
    uint32_t addr;
    char buf[MAXBUF];

    if(argc!=2){
        exit(0);
    }
    // 0x -> 0.0.0.0
    sscanf(argv[1],"%x",&addr);
    inaddr.s_addr=htonl(addr);
    if(!inet_ntop(AF_INET,&inaddr,buf,MAXBUF)){
        unix_error("inet_ntop");
    }
    printf("%s\n",buf);
    exit(0);
}
/*
107.212.122.205
8f d4  8a cd

64.12.149.13
40 0c 95 0d

107.212.96.29
6b d4 60 1D

0x00000080
0.0.0.128

0xFFFFFF00 16*15+15
255.255.255.0

0x0A 01 01 40
10.1.1.64


*/