#include "chip8.h"
#include "instruction_set.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>

#define DEBUG 1

void init_chip8(Chip8* system)
{
    system->pc = 0x200;
    system->I = 0;
    system->sp = 0;
    system->dt = 0;
    system->st = 0;
    system->draw_flag = 0;
    system->sound_flag = 0;
    system->debug_flag = DEBUG;

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            system->display[i][j] = 0;
        }
    }

    for (int i = 0; i < sizeof(system->memory); i++) {
        system->memory[i] = 0;
    }

    for (int i = 0; i < sizeof(system->V); i++) {
        system->V[i] = 0;
    }

    for (int i = 0; i < sizeof(system->stack); i++) {
        system->stack[i] = 0;
    }

    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80   // F
    };

    for (int i = 0; i < 80; i++) {
        system->memory[i] = fontset[i];
    }

}

int load_rom(Chip8* system, char* filename)
{
    FILE* rom = fopen(filename, "rb");
    if (rom == NULL) {
        printf("ERROR: ROM file not loaded");
        exit(EXIT_FAILURE);
    }

    fseek(rom, 0, SEEK_END);
    long rom_length = ftell(rom);
    rewind(rom);

    uint8_t* rom_buffer = (uint8_t*)malloc(sizeof(uint8_t) * rom_length);
    if (rom_buffer == NULL) {
        printf("ERROR: Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    fread(rom_buffer, sizeof(uint8_t), rom_length, rom);

    for (int i = 0; i < rom_length; i++) {
        system->memory[i+0x200] = rom_buffer[i];
    }
    fclose(rom);
    free(rom_buffer);
}

void emulate_cycle(Chip8* system)
{
    system->draw_flag = 0;
    system->sound_flag = 0;

    uint16_t op_code = system->memory[system->pc] << 8 | system->memory[system->pc+1];
    uint16_t x = (op_code & 0x0F00) >> 8;
    int16_t y = (op_code & 0x00F0) >> 4;
    
    printf("pc: 0x%X\n", system->pc);
    printf("opcode: 0x%X\n", op_code);
    printf("opcode: 0x%X\n", x);
    printf("opcode: 0x%X\n", y);

    // Mask to get "first nibble"
    switch(op_code & 0xF000) {
        case 0x0000:
            switch (op_code & 0x00FF) {
                // 00E0: Clear screen
                case 0x00E0:
                    op_clear_screen(system, &op_code);
                    break;

                case 0x00EE:
                    printf("[OK] 0x%X: 00EE\n", op_code);
                    system->sp--;
                    system->pc = system->stack[system->sp];
                    system->pc += 2;
                    break;
                
                default:
                    printf("[FAILED] Unknown opcode: 0x%X\n", op_code);
                    break;
            }
            system->pc += 2;
            break;
        
        // 1NNN: Jump to NNN
        case 0x1000:
            printf("[OK] 0x%X: 1NNN\n", op_code);
            system->pc = op_code & 0x0FFF;
            break;

        // 6XNN: Set Vx equal to NN
        case 0x6000:
            printf("[OK] 0x%X: 6XNN\n", op_code);
            system->V[x] = (op_code & 0x00FF);
            system->pc += 2;
            break;

        // 7XNN: Add NN to X
        case 0x7000:
            printf("[OK] 0x%X: 7XNN\n", op_code);
            system->V[x] = system->V[x] + (op_code & 0x00FF);
            system->pc += 2;
            break;
        
        // ANNN: Set index register I
        case 0xA000:
            printf("[OK] 0x%X: ANNN\n", op_code);
            system->I = (op_code & 0x0FFF);
            system->pc += 2;
            break;

        // DXYN: Display/draw
        case 0xD000:
            printf("[OK] 0x%X: DXYN\n", op_code);

            uint16_t x_loc = system->V[x];
            uint16_t y_loc = system->V[y];
            uint16_t sprite_height = (op_code & 0x000F);
            
            // Set collision flag to false (default)
            system->V[0xF] = 0;
            
            uint16_t px;

            for (int yline = 0; yline < sprite_height; yline++) {
                // Pixel value from memory at location I
                px = system->memory[system->I + yline];

                // Loop through 8 bits of row
                for (int xline = 0; xline < 8; xline++) {
                    if ((px & (0x80 >> xline)) != 0) {
                        if (system->display[y_loc + yline][x_loc + xline] == 1) {
                            system->V[0xF] = 1;
                        }
                        system->display[y_loc + yline][x_loc + xline] ^= 1;
                    }
                }
            }
            system->draw_flag = 1;
            system->pc += 2;
            break;

        // ERROR
        default:
            printf("Not implemented yet...");
            system->pc += 2;
            break;
    }
}