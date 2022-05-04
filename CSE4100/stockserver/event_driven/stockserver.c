#include "stockserver.h"



int main(int argc,char** argv){
    read_stockfile();
    if(argc!=2) error_exit("usage : ./stockserver {port}");
    listenfd=open_listenfd(argv[1]);
    init_pool(listenfd);
    while(1){
        see_pool();
        write_pool();
        if(_pool.n==0||time_check()>MAX_FSYNC_TIME){
            fsync_stockfile();
        }
    }
}


