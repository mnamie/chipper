#include "instruction_set.h"
#include "chip8.h"

#include <stdio.h>

// 00E0: Clear screen
void op_clear_screen(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 00EO\n", *op_code); }
    system->draw_flag = 1; // Set draw_flag to render

    // Zero display to clear
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            system->display[y][x] = 0;
        }
    }
    system->pc += 2;
}