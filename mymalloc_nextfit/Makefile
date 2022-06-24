CC = gcc
CFLAGS = -Wall -O2 -m32
TESTFLAGS= -Wall -O2 -m32 -DTEST
OBJS = mdriver.o mm.o memlib.o fsecs.o fcyc.o clock.o ftimer.o
TESTOBJS= test.o mdriver.o mm.o memlib.o fsecs.o fcyc.o clock.o ftimer.o 

test: CLFAGS+=-DTEST

mdriver: CFLAGS+=-DPRODUCTION
debug: CFLAGS+=-DPRODUCTION

mdriver: $(OBJS)
	$(CC) $(CFLAGS) -o mdriver $(OBJS)

debug: $(OBJS)
	$(CC) $(CFLAGS) -g -o debug $(OBJS)

test: $(TESTOBJS)
	$(CC) $(TESTFLAGS) -o test $(TESTOBJS) 

mdriver.o: mdriver.c fsecs.h fcyc.h clock.h memlib.h config.h mm.h
memlib.o: memlib.c memlib.h
mm.o: mm.c mm.h memlib.h
fsecs.o: fsecs.c fsecs.h config.h
fcyc.o: fcyc.c fcyc.h
ftimer.o: ftimer.c ftimer.h config.h
clock.o: clock.c clock.h
test.o: test.c

clean:
	rm -f *~ *.o mdriver test
