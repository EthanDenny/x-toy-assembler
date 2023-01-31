CC = g++
CFLAGS = -g #-Wall

default: parser

parser:	parser.o
	$(CC) $(CFLAGS) -o parser parser.o

parser.o: parser.cpp types.h
	$(CC) $(CFLAGS) -c parser.cpp

clean:
	rm parser *.o
