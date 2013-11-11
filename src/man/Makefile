# C Compiler
CC = gcc
# C Flags
CFLAGS=-g -I./common/ `pkg-config --cflags x11`
# LD Flags
LDFLAGS=`pkg-config --libs x11`
# Objects
OBJS=common/properties.o atoms.o window.o frame.o manager.o

all: main atoms window frame manager common
	$(CC) -o space-man $(CFLAGS) main.o $(OBJS) $(LDFLAGS)

main: main.c
	$(CC) -c main.c $(CFLAGS)

atoms: atoms.h atoms.c
	$(CC) -c atoms.c $(CFLAGS)

window: window.h window.c
	$(CC) -c window.c $(CFLAGS)

frame: frame.h frame.c
	$(CC) -c frame.c $(CFLAGS)

manager: manager.h manager.c
	$(CC) -c manager.c $(CFLAGS)

test: properties atoms manager test.c
	$(CC) -o test test.c $(CFLAGS) $(OBJS) $(LDFLAGS)

clean:
	rm *.o space-man