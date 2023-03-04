CC = g++
CFLAGS = -g -Wall

default: main

main: main.o exception.o lexer.o parser.o
	$(CC) $(CFLAGS) main.o exception.o lexer.o parser.o -o main

main.o: parser.h lexer.h types.h
	$(CC) $(CFLAGS) -c main.cpp

parser: parser.o lexer.o
	$(CC) $(CFLAGS) parser.o lexer.o -o parser

parser.o: parser.cpp exception.h lexer.h types.h
	$(CC) $(CFLAGS) -c parser.cpp

lexer: lexer.o
	$(CC) $(CFLAGS) lexer.o -o lexer

lexer.o: lexer.cpp exception.h types.h
	$(CC) $(CFLAGS) -c lexer.cpp

clean:
	-del main parser lexer exception *.o *.exe *.out
