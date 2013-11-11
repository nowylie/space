# C Compiler
CC = gcc
# C Flags
CFLAGS=-g `pkg-config --cflags x11`
# LD Flags
LDFLAGS=`pkg-config --libs x11`

all: properties.o

properties.o: properties.h properties.c
	$(CC) -c properties.c $(CFLAGS)

clean:
	rm *.o
