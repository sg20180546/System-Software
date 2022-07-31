
-------------------------------------------------------------
## This repository is based on [CSAPP, Computer Systems: A Programmer's Perspective, Global Edition (3rd Edition)](https://csapp.cs.cmu.edu/).

## 1. Myshell project : Make my custom shell by linux system calls
1) fork and execute

![image](https://user-images.githubusercontent.com/81512075/182013592-4a958cd9-9b3b-430d-acda-f32053c0b16b.png)

2) pipelining

![image](https://user-images.githubusercontent.com/81512075/182013610-a41815dd-6b7f-43bd-8baa-ef8f0f09ed58.png)

3) background job

![image](https://user-images.githubusercontent.com/81512075/182013616-9378e46a-4161-4d51-a17d-51223da5b448.png)

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
        ![image](https://user-images.githubusercontent.com/81512075/182013574-2dbe5d66-9330-4cca-af66-d48a6ed5a948.png)

        - you can test server throughput by /test/common_test/benchmark.c
        - to test, compile /client/multiclient_behchmark.c (just run make !)
* 4) Common
        - stock file format is "{ID} {COUNT} {PRICE}\n", saved at same path of server process.
        - when server boot, stock file is loaded on memory as shape as binary tree structure.


## 3-1. Mymalloc_buddy : Physical Memory allocating Simulator 

![image](https://user-images.githubusercontent.com/81512075/182013679-484345fe-d06b-4fba-aba0-7eca2d194a0e.png)

- Based on Linux Kernel Buddy System, which allocate Page(4KB) to process
- Mymalloc customization : Total 20MB Phiscal Memory,64 Byte Page size, 2^24 Byte Physical Area 
- Total 65536 Pages, 16 Orders
- But in malloc lab, Space utilization Performnace is mesured by static char* mem_brk which mean buddy system cannnot properly evalutated in mem lab

## 3-2. Mymalloc_next fit
- which is combined version of first fit and best fit

## 4. CSE3030, CSE4040 is class material of Sogang University Computer Sci. Course.
- CSE3030(Introduction of Computer System)
- CSE4040(System Programming)

## 5. excode
- example code of CSAPP book.
