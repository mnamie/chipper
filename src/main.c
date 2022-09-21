#include "chip8.h"
#include "gfx.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: chip8 rom.ch8");
        return 1;
    }

    Chip8 chip8;
    init_chip8(&chip8);
    load_rom(&chip8, argv[1]);

    init_display();

    while (1) {
        emulate_cycle(&chip8);
        
        if (chip8.draw_flag) {
            buffer_draw(&chip8);
            draw(&chip8);
        }

        usleep(1500);
    }

    return 0;
}