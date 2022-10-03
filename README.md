# Chipper

Yet another CHIP8 emulator for Linux!

### Instructions:

1. Clone repository
2. Install libsdl2-dev (`sudo apt-get install libsdl2-dev` on Ubuntu, for example)
3. Run `make` in the top directory
4. Start playing with `./chipper 1500 0 path_to_rom.ch8`

(`./chipper [int: delay_timer] [int: debug_flag] [str: path_to_rom]`)

### Current Status:

All Op-Codes implemented

Passing CHIP-8 test ROMs

### To-Do:

1. There appears to be some error in one of the draw opcodes causing issues during certain segments of gameplay
2. Sound
3. Color pallette selection
