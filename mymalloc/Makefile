CC = gcc
CFLAGS = -Wall -O2 -m32
OBJS = mdriver.o mm.o memlib.o fsecs.o fcyc.o clock.o ftimer.o 
TESTOBJS= test.o mdriver.o mm.o memlib.o fsecs.o fcyc.o clock.o ftimer.o 
OBJECT_DIR=./build
# test: CFLAGS+=-DTEST

test: CLFAGS+=-DTEST

mdriver: CFLAGS+=-DPRODUCTION

mdriver: $(OBJS)
	$(CC) $(CFLAGS) -o mdriver $(OBJS)
debug: $(OBJS)
	$(CC) $(CFLAGS) -g -o debug $(OBJS)
test: $(TESTOBJS)
	$(CC) $(CFLAGS) -o test $(TESTOBJS) 


mdriver.o: mdriver.c fsecs.h fcyc.h clock.h memlib.h config.h mm.h
memlib.o: memlib.c memlib.h
mm.o: mm.c mm.h memlib.h
fsecs.o: fsecs.c fsecs.h config.h
fcyc.o: fcyc.c fcyc.h
ftimer.o: ftimer.c ftimer.h config.h
clock.o: clock.c clock.h



test.o: test.c
clean:
	rm -f *~ *.o mdriver
