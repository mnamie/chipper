#include "chip8.h"
#include "gfx.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    // Check argc to ensure arg was provided for rom filepath
    if (argc != 2) {
        printf("Usage: chip8 rom.ch8");
        return 1;
    }

    // Init chip8 system struct and initialize
    Chip8 chip8;
    init_chip8(&chip8);

    // Load roam into chip8 system
    load_rom(&chip8, argv[1]);

    // Init SDL2 display
    init_display();

    // Emulation loop
    while (1) {
        // Fetch, decode, execute cycle
        emulate_cycle(&chip8);
        
        // Draw if flag is set
        if (chip8.draw_flag) {
            buffer_draw(&chip8);
            draw(&chip8);
        }

        // Timing
        usleep(1500);
    }

    return 0;
}