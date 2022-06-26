
-------------------------------------------------------------
## This repository is based on [CSAPP, Computer Systems: A Programmer's Perspective, Global Edition (3rd Edition)](https://csapp.cs.cmu.edu/).

## 1. Myshell project : Make my custom shell by linux system calls
- Shell interpret user command, fork process, finally execute builtin command on forked process.
- To enable IPC(interprocess communication), Signal and Pipe are used.
- SIGCHLD,SIGTTOU,SIGTTIN,SIGTSTP,SIGINT is mainly-dealing-with signal.
- For most, SIGCHILD handler must reap child process to free "MEMORY IN THE KERNEL ADDRESS SPACE", such as thread_union,thread_info and task_struct which allocated for management thread(process) in kernel address space.

## 2. Concurrent stockserver project : Make Conccurent two type of Concurrent server, one is EVENT_DRIVEN, the other is THREAD_BASED.
* 1) EVENT_DRIVEN
        - EVENT_DRIVEN server run sequntially at ONE PROCESS.
        - select(), epoll() function is key function, which request KERNEL to check CONNECTION camed.
* 2) THREAD_BASED
        - THREAD_BASED server consist of ONE MAIN thread and N WORKER threads.
        - MAIN thread push CONNECTION FILE DESCRIPTOR at SBUF, which use mutex to prevent concurrency bugs.
        - N WORKER threads gain CONNETION FD from SBUF, and then do network communcation.
        - as This server use binary tree in heap, to prevent reader-writer problem, There are both lock of each node, and that of whole binary tree
* 3) Benchmarking
        - you can test server throughput by /test/common_test/benchmark.c
        - to test, compile /client/multiclient_behchmark.c (just run make !)
* 4) Common
        - stock file format is "{ID} {COUNT} {PRICE}\n", saved at same path of server process.
        - when server boot, stock file is loaded on memory as shape as binary tree structure.

## 3-1. Mymalloc_buddy : Physical Memory allocating Simulator 
- Based on Linux Kernel Buddy System, which allocate Page(4KB) to process
- Mymalloc customization : Total 20MB Phiscal Memory,64 Byte Page size, 2^24 Byte Physical Area 
- Total 65536 Pages, 16 Orders
- But in malloc lab, Space utilization Performnace is mesured by static char* mem_brk which mean buddy system cannnot properly evalutated in mem lab

## 3-2. Mymalloc_next fit
- which is combined version of first fit and best fit

## 5. CSE3030, CSE4040 is class material of Sogang University Computer Sci. Course.
- CSE3030(Introduction of Computer System)
- CSE4040(System Programming)

## 6. excode
- example code of CSAPP book.
