# C Compiler
CC = gcc
# C Flags
CFLAGS=-Wall -g `pkg-config --cflags x11 xext xcomposite xrender xdamage xfixes`
# LD Flags
LDFLAGS=-lpthread `pkg-config --libs x11 xext xcomposite xrender xdamage xfixes`
# Objects
OBJS=event_thread.o scene.o viewport.o object.o window.o

all: main event_thread scene viewport object window
	$(CC) -o space $(CFLAGS) main.o $(OBJS) $(LDFLAGS)

main: common.h main.c
	$(CC) -c main.c $(CFLAGS)

event_thread: common.h event_thread.h event_thread.c
	$(CC) -c event_thread.c $(CFLAGS)

scene: common.h scene.h scene.c
	$(CC) -c scene.c $(CFLAGS)

viewport: common.h viewport.h viewport.c
	$(CC) -c viewport.c $(CFLAGS)

object: common.h object.h object.c
	$(CC) -c object.c $(CFLAGS)

window: common.h window.h window.c
	$(CC) -c window.c $(CFLAGS)

test: all test.c
	$(CC) -o test test.c $(CFLAGS) $(OBJS) $(LDFLAGS)