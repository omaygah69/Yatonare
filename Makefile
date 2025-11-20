CC = g++
CFLAGS = -lraylib -Iinclude
SRC = main.cpp noise.cpp gui.cpp mesh.cpp
BUILDDIR = build

all: main
	./$(BUILDDIR)/main
main: main.cpp
	$(CC) $(CFLAGS) $(SRC) -o $(BUILDDIR)/main

clean:
	rm -rf main
