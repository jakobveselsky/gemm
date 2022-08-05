CC = gcc
CFLAGS=  -O0 -lm -lpthread -g

all: queuepthreads

clean:
	rm -f queuepthreads.o

queuepthreads: queuepthreads.c
	$(CC) buffer.c queuepthreads.c -o queuepthreads.o $(CFLAGS)