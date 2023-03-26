CC = gcc
COMPILER_FLAGS = -Wall
LINKER_FLAGS = -lSDL2

build/chipper: build/main.o build/chip8.o build/io.o build/instruction_set.o
	@$(CC) $(COMPILER_FLAGS) -o ./bin/chipper build/main.o build/chip8.o build/io.o build/instruction_set.o $(LINKER_FLAGS)

build/main.o:	src/main.c
	@$(CC) $(COMPILER_FLAGS) -c -o build/main.o src/main.c

build/chip8.o: src/chip8.c
	@$(CC) $(COMPILER_FLAGS) -c -o build/chip8.o src/chip8.c

build/io.o: src/io.c
	@$(CC) $(COMPILER_FLAGS) -c -o build/io.o src/io.c

build/instruction_set.o: src/instruction_set.c
	@$(CC) $(COMPILER_FLAGS) -c -o build/instruction_set.o src/instruction_set.c

build_dir:
	@mkdir ./build
	@mkdir ./bin

clean:
	@rm -rf ./build
	@rm -rf ./bin

.PHONY: build_dir clean
