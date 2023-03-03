CC = g++
CFLAGS = -g -Wall

default: parser

assembler: assembler.o
	$(CC) $(CFLAGS) assembler.o -o assembler

assembler.o: assembler.cpp types.h
	$(CC) $(CFLAGS) -c assembler.cpp

parser: parser.o
	$(CC) $(CFLAGS) parser.o -o parser

parser.o: parser.cpp types.h
	$(CC) $(CFLAGS) -c parser.cpp

clean:
	rm assembler parser *.o
