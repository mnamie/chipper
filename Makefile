.PHONY: build_dir

CC = gcc
COMPILER_FLAGS = -Wall
LINKER_FLAGS = -lSDL2

build/chipper: build/main.o build/chip8.o build/io.o build/instruction_set.o
	@gcc -o ./chipper build/main.o build/chip8.o build/io.o build/instruction_set.o $(LINKER_FLAGS)

build/main.o:	src/main.c
	@gcc -c -o build/main.o src/main.c

build/chip8.o: src/chip8.c
	@gcc -c -o build/chip8.o src/chip8.c

build/io.o: src/io.c
	@gcc -c -o build/io.o src/io.c

build/instruction_set.o: src/instruction_set.c
	@gcc -c -o build/instruction_set.o src/instruction_set.c

build_dir:
	@mkdir ./build
