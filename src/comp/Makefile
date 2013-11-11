# C Compiler
CC = gcc
# C Flags
CFLAGS=-Wall -g `pkg-config --cflags x11 xext xcomposite xrender xdamage xfixes`
# LD Flags
LDFLAGS=`pkg-config --libs x11 xext xcomposite xrender xdamage xfixes`
# Objects
OBJS=event_thread.o window.o

all: main event_thread window
	$(CC) -o space-comp $(CFLAGS) main.o $(OBJS) $(LDFLAGS)

main: common.h main.c
	$(CC) -c main.c $(CFLAGS)

event_thread: common.h event_thread.h event_thread.c
	$(CC) -c event_thread.c $(CFLAGS)

window: common.h window.h window.c
	$(CC) -c window.c $(CFLAGS)

test: all test.c
	(CC) -o test test.c $(CFLAGS) $(OBJS) $(LDFLAGS)

clean:
	rm *.o space-comp
