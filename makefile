CC = g++
CFLAGS = -g #-Wall

default: assembler

assembler: assembler.o
	$(CC) $(CFLAGS) -o assembler assembler.o

assembler.o: assembler.cpp types.h
	$(CC) $(CFLAGS) -c assembler.cpp

clean:
	rm assembler *.o
