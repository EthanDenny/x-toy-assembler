CC = g++
CFLAGS = -g -Wall

default: lexer

assembler: assembler.o
	$(CC) $(CFLAGS) assembler.o -o assembler

assembler.o: assembler.cpp types.h
	$(CC) $(CFLAGS) -c assembler.cpp

lexer: lexer.o
	$(CC) $(CFLAGS) lexer.o -o lexer

lexer.o: lexer.cpp types.h
	$(CC) $(CFLAGS) -c lexer.cpp

clean:
	-del assembler lexer *.o *.exe *.out
