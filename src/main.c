#include "chip8.h"
#include "io.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

extern SDL_Event e;

int
main(int argc, char** argv)
{
    // Check argc to ensure arg was provided for rom filepath
    if (argc != 4) {
        printf("Usage: chip8 [int: delay_timer] [int: debug_flag] [string: rom.ch8]\n");
        return 1;
    }

    int delay_timer = atoi(argv[1]);
    int debug_flag = atoi(argv[2]);

    // Init chip8 system struct and initialize
    Chip8 chip8;
    init_chip8(&chip8, debug_flag);

    // Load roam into chip8 system
    load_rom(&chip8, argv[3]);

    init_display("Chipper");

    // Emulation loop
    unsigned int run = 1;
    
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
        usleep(delay_timer);
    }

    return 0;
}