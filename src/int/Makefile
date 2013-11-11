# C Compiler
CC = gcc
# C Flags
CFLAGS=-g -I./common/ `pkg-config --cflags x11`
# LD Flags
LDFLAGS=-lm `pkg-config --libs x11`
# Objects
OBJS=common/properties.o button.o message.o

all: main button message
	$(CC) -o space-int $(CFLAGS) main.o $(OBJS) $(LDFLAGS)

main: main.c
	$(CC) -c main.c $(CFLAGS)

button: button.h button.c
	$(CC) -c button.c $(CFLAGS)

message: message.h message.c
	$(CC) -c message.c $(CFLAGS)

clean:
	rm *.o space-int