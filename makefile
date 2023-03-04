# Compiler and linker settings
CC = g++
CFLAGS = -g -Wall
LFLAGS = 

# Directories
SRCDIR = src
BUILDDIR = build

# Files to compile
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# Name of the executable
TARGET = $(BUILDDIR)\main

default: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LFLAGS) $^ -o $@
	-del $(BUILDDIR)\*.o

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-del $(BUILDDIR)\*.o $(TARGET)
	-del $(TARGET).exe
