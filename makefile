# Makefile for Program 3 (Readers and Writers) CS570 Fall 2015

CC = gcc
CFLAGS = -g -O -Wall -Wpointer-arith -Wcast-qual -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wnested-externs -D_REENTRANT

# The first target in this makefile is 'test', so 'make' will run the test
# of your p3 with parameter '109'.  Since p3 is listed as a dependency, the
# test will first make sure p3 is up to date before running the test.
# If you only want to compile rather than run the test, use 'make p3' .

test:	p3
	@echo Now running p3 with a seed of 40:
	./p3 40

p3:	p3main.o p3helper.o
	${CC} -mt -o p3 p3main.o p3helper.o -lpthread -lposix4

p3main.o:	p3.h

p3helper.o:	p3.h

clean:
	rm -f p3 p3main.o p3helper.o 

test2:	p3
	./p3 6
