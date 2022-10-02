#include "chip8.h"
#include "io.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    // Check argc to ensure arg was provided for rom filepath
    if (argc != 2) {
        printf("Usage: chip8 rom.ch8");
        return 1;
    }

    // Init chip8 system struct and initialize
    Chip8 chip8;
    init_chip8(&chip8, 1);

    // Load roam into chip8 system
    load_rom(&chip8, argv[1]);

    init_display("Chipper");

    // Emulation loop
    uint run = 1;
    SDL_Event e;
    while (run) {
        // Event polling from SDL
        run = process_input(&chip8, &e);
        
        // Fetch, decode, execute cycle
        emulate_cycle(&chip8);
        
        // Draw if flag is set
        if (chip8.draw_flag) {
            buffer_draw(&chip8);
            draw(&chip8);
        }

        // Timing
        usleep(2);
    }

    return 0;
}