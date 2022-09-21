#include "instruction_set.h"
#include "chip8.h"

#include <stdio.h>

// 00E0: Clear screen
void op_clear_screen(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 00EO\n", *op_code); }
    system->draw_flag = 1;
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            system->display[i][j] = 0;
        }
    }
    system->pc += 2;
}