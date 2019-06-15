
CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99

all: buNeDuFork

buNeDuFork: hw2.o zEvaluate.o
	 $(CC) -o buNeDuFork hw2.o zEvaluate.o

hw1.o: hw2.c
	 $(CC) $(CFLAGS) hw2.c zEvaluate.h

zEvaluate.o: zEvaluate.c
	 $(CC) $(CFLAGS) zEvaluate.c zEvaluate.h

clean: 
	rm *.o buNeDuFork
