CC = g++
CFLAGS = -g -Wall

default: main

main: main.o parser.o lexer.o
	$(CC) $(CFLAGS) main.o parser.o lexer.o -o main

main.o: parser.h lexer.h types.h
	$(CC) $(CFLAGS) -c main.cpp

parser: parser.o lexer.o
	$(CC) $(CFLAGS) parser.o lexer.o -o parser

parser.o: parser.cpp lexer.h types.h
	$(CC) $(CFLAGS) -c parser.cpp

lexer: lexer.o
	$(CC) $(CFLAGS) lexer.o -o lexer

lexer.o: lexer.cpp types.h
	$(CC) $(CFLAGS) -c lexer.cpp

clean:
	-del main parser lexer *.o *.exe *.out
