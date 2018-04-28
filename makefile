CC = g++
BUILDDIR = build
SRCDIR = src
CFLAGS = -lglut -lGL -lGLU -lGLEW
INC = -Iinclude
TARGET = bin/output
OBJECTS = $(shell find $(SRCDIR) -name "*.cpp" | sed -r "s|$(SRCDIR)/([a-zA-Z]+).cpp|$(BUILDDIR)/\1.o|")

default: main

main: $(OBJECTS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INC) -o $(TARGET) $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@echo "Removing $(BUILDDIR)/"; rm -r $(BUILDDIR)
