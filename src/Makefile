# C Compiler
CC = gcc
# C Flags
CFLAGS=-Wall -g `pkg-config --cflags x11 xext xcomposite xrender xdamage xfixes`
# LD Flags
LDFLAGS=-lpthread `pkg-config --libs x11 xext xcomposite xrender xdamage xfixes`
# Objects
OBJS=render_thread.o event_thread.o scene.o viewport.o object.o

all: main render_thread event_thread scene viewport object
	$(CC) -o space $(CFLAGS) main.o $(OBJS) $(LDFLAGS)

main: common.h main.c
	$(CC) -c main.c $(CFLAGS)

render_thread: common.h render_thread.h render_thread.c
	$(CC) -c render_thread.c $(CFLAGS)

event_thread: common.h event_thread.h event_thread.c
	$(CC) -c event_thread.c $(CFLAGS)

scene: common.h scene.h scene.c
	$(CC) -c scene.c $(CFLAGS)

viewport: common.h viewport.h viewport.c
	$(CC) -c viewport.c $(CFLAGS)

object: common.h object.h object.c
	$(CC) -c object.c $(CFLAGS)

test: all test.c
	$(CC) -o test test.c $(CFLAGS) $(OBJS) $(LDFLAGS)