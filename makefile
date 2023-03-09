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
ifeq ($(OS), Windows_NT)
    TARGET = $(BUILDDIR)\main
else
    TARGET = $(BUILDDIR)/main
endif

default: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LFLAGS) $^ -o $@

    ifeq ($(OS), Windows_NT)
		-del $(BUILDDIR)\*.o
    else
		-rm -f $(BUILDDIR)/*.o
    endif

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
    ifeq ($(OS), Windows_NT)
		if not exist build mkdir build
    else
		mkdir -p build
    endif
	$(CC) $(CFLAGS) -c $< -o $@

clean:
    ifeq ($(OS), Windows_NT)
		-del $(BUILDDIR)\*.o $(TARGET) $(TARGET).exe
    else
		-rm -f $(BUILDDIR)/*.o $(TARGET) $(TARGET).exe
    endif
