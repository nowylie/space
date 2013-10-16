# C Compiler
CC = gcc
# C Flags
CFLAGS=-Wall -ansi -g `pkg-config --cflags x11 xcomposite cairo`
# LD Flags
LDFLAGS=-lpthread `pkg-config --libs x11 xcomposite cairo` 
# Objects
OBJS=main.o render_thread.o event_thread.o

all: main render_thread event_thread
	$(CC) -o space $(CFLAGS) $(OBJS) $(LDFLAGS)

main: common.h main.c
	$(CC) -c main.c $(CFLAGS)

render_thread: common.h render_thread.h render_thread.c
	$(CC) -c render_thread.c $(CFLAGS)

event_thread: common.h event_thread.h event_thread.c
	$(CC) -c event_thread.c $(CFLAGS)