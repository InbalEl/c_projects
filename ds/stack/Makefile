CC=gcc
CFLAGS=-I

.PHONY: all clean

all: stack.o stack_test.o
	$(CC) -o stack_test.out stack.o stack_test.o

stack.o: stack.c stack.h
	$(CC) -c stack.c

stack_test.o: stack_test.c stack.h
	$(CC) -c stack_test.c

clean:
	rm -f *.o *.out
