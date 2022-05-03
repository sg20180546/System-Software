#include "stockserver.h"



int main(){
    read_stockfile();
    show(1);

    // while(1){
    //     event_driven();
    //     if(if_all_connection_close()||time_check()>MAX_FSYNC_TIME){
    //         fsync_stockfile();
    //     }
    // }
}



// void error_exit(char* msg){
//     fprintf(stderr,msg);
//     exit(0);
// }
