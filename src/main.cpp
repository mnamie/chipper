#include "chip8.hpp"
#include "io.hpp"

#include <SDL2/SDL.h>
#include <cstdio>
#include <unistd.h>

int main(int argc, char** argv)
{
    // Check argc to ensure arg was provided for rom filepath
    if (argc != 4) {
        printf("Usage: chip8 [int: delay_timer] [int: debug_flag] [string: rom.ch8]\n");
        return 1;
    }

    int delay_timer = atoi(argv[1]);
    int debug_flag = atoi(argv[2]);

    // Init display for IO
    Display io("Chipper");

    // Init chip8 system struct and initialize
    Chip8 chip8(debug_flag, &io);

    // Load roam into chip8 system
    chip8.loadRom(argv[3]);

    // Emulation loop
    bool run = 1;
    while (run) {
        // Event polling from SDL
        run = io.processInput(&chip8);
        
        // Fetch, decode, execute cycle
        chip8.emulateCycle();
        
        // Draw if flag is set
        if (chip8.draw_flag) {
            io.bufferDraw(&chip8);
            io.draw(&chip8);
        }

        // Timing
        usleep(delay_timer);
    }

    io.destroy();

    return 0;
}