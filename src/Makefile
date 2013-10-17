# C Compiler
CC = gcc
# C Flags
CFLAGS=-Wall -ansi -g `pkg-config --cflags x11 xcomposite cairo`
# LD Flags
LDFLAGS=-lpthread `pkg-config --libs x11 xcomposite cairo` 
# Objects
OBJS=render_thread.o event_thread.o window_hash.o

all: main render_thread event_thread window_hash
	$(CC) -o space $(CFLAGS) main.o $(OBJS) $(LDFLAGS)

main: common.h main.c
	$(CC) -c main.c $(CFLAGS)

render_thread: common.h render_thread.h render_thread.c
	$(CC) -c render_thread.c $(CFLAGS)

event_thread: common.h event_thread.h event_thread.c
	$(CC) -c event_thread.c $(CFLAGS)

window_hash: common.h window_hash.h window_hash.c
	$(CC) -c window_hash.c $(CFLAGS)

test: all test.c
	$(CC) -o test test.c $(CFLAGS) $(OBJS) $(LDFLAGS)