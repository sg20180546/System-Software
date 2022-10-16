
CC=gcc-9
INCLUDE=-I./include
OBJDIR=./objdir
CFLAGS=-c -o
MODULEDIR=./module


.PHONY:myshell
myshell : myshell.o interpreter.o builtin.o common.o execute.o sig.o cd.o exit.o jobs.o
	$(CC) -o $@ $^  -I./include/ 



debug : myshell.o interpreter.o builtin.o common.o execute.o sig.o cd.o exit.o jobs.o
	CFLAGS+=-g

sig.o : sig.c
	$(CC) $(CFLAGS) $@ $< $(INCLUDE)

execute.o : execute.c
	$(CC) $(CFLAGS) $@ $< $(INCLUDE) 

common.o : common.c
	$(CC) $(CFLAGS) $@ $< $(INCLUDE)

builtin.o : builtin.c
	$(CC) $(CFLAGS) $@ $< $(INCLUDE)

myshell.o: myshell.c
	$(CC) $(CFLAGS) $@ $< $(INCLUDE)

cd.o: $(MODULEDIR)/cd.c
	$(CC) $(CFLAGS) $@ $< $(INCLUDE) 

exit.o: $(MODULEDIR)/exit.c
	$(CC) $(CFLAGS) $@ $< $(INCLUDE)

jobs.o: $(MODULEDIR)/jobs.c
	$(CC) $(CFLAGS) $@ $< $(INCLUDE)

interpreter.o : interpreter.c
	$(CC) $(CFLAGS) $@ $< $(INCLUDE)

clean :
	rm myshell debugshell *.o
tar :
	tar -cvzf ${PWD}/../../20180546.tar.gz ${PWD}/../../20180546
rm_tar :
	rm ${PWD}/../../20180546.tar.gz
scp :
	scp ${PWD}/../../20180546.tar.gz cse20180546@cspro.sogang.ac.kr:/sogang/under/cse20180546