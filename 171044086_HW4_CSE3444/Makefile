
CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99

all: buNeDuFPF

buNeDuFPF: hw4.o zEvaluate.o
	 $(CC) -o buNeDuFPF hw4.o zEvaluate.o

hw4.o: hw4.c
	 $(CC) $(CFLAGS) hw4.c zEvaluate.h

zEvaluate.o: zEvaluate.c
	 $(CC) $(CFLAGS) zEvaluate.c zEvaluate.h

clean: 
	rm *.o buNeDuFPF
