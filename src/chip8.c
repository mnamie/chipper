#include "chip8.h"
#include "instruction_set.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>

#define KEY_DEBUG 1

// Initialize Chip8 system data structure
void init_chip8(Chip8* system)
{
    system->pc = 0x200;
    system->I = 0;
    system->sp = 0;
    system->dt = 0;
    system->st = 0;
    system->draw_flag = 0;
    system->sound_flag = 0;
    system->debug_flag = KEY_DEBUG; // Debug flag dictates console output

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

    // Copy fontset to memory
    for (int i = 0; i < 80; i++) {
        system->memory[i] = fontset[i];
    }

    // Initialize keypad
    for (int i = 0; i < KEYPAD_SIZE; i++) {
        system->keypad[i] = 0;
    }

}

int load_rom(Chip8* system, char* filename)
{
    FILE* rom = fopen(filename, "rb");
    if (rom == NULL) {
        printf("ERROR: ROM file not loaded");
        exit(EXIT_FAILURE);
    }

    // Get length of rom
    fseek(rom, 0, SEEK_END);
    long rom_length = ftell(rom);
    rewind(rom);

    // Initilize rom buffer
    uint8_t* rom_buffer = (uint8_t*)malloc(sizeof(uint8_t) * rom_length);
    if (rom_buffer == NULL) {
        printf("ERROR: Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    // Read rom into rom_buffer
    fread(rom_buffer, sizeof(uint8_t), rom_length, rom);

    // Copy rom_buffer to system memory
    for (int i = 0; i < rom_length; i++) {
        system->memory[i+0x200] = rom_buffer[i];
    }

    // Cleanup
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
    
    if (system->debug_flag) {
        printf("pc: 0x%X\n", system->pc);
        printf("opcode: 0x%X\n", op_code);
        printf("opcode: 0x%X\n", x);
        printf("opcode: 0x%X\n", y);
    }
    
    // Mask to get "first nibble"
    switch (op_code & 0xF000) {
        case 0x0000:
            switch (op_code & 0x00FF) {
                // 00E0: Clear screen
                case 0x00E0:
                    op_cls(system, &op_code);
                    break;

                // OOEE: Return
                case 0x00EE:
                    op_ret(system, &op_code);
                    break;
                
                default:
                    printf("[FAILED] Unknown opcode: 0x%X\n", op_code);
                    break;
            }
            break;
        
        // 1NNN: Jump to NNN
        case 0x1000:
            op_jp(system, &op_code);
            break;

        // 2NNN: Call
        case 0x2000:
            op_call(system, &op_code);
            break;

        // 3XNN: Skip next if x == NN
        case 0x3000:
            op_se(system, &op_code);
            break;

        // 4XNN: Skip next if x != NN
        case 0x4000:
            op_sne(system, &op_code);
            break;

        // 5XY0: Skip next insturction if Vx == Vy
        case 0x5000:
            op_se_compare(system, &op_code);
            break;

        // 6XNN: Set Vx equal to NN
        case 0x6000:
            op_ld_vx(system, &op_code);
            break;

        // 7XNN: Add NN to X
        case 0x7000:
            op_add(system, &op_code);
            break;

        // 8000 Cases
        case 0x8000:
            switch (op_code & 0x000F) {
                case 0x0000:
                    op_ld_vx_vy(system, &op_code);
                    break;

                case 0x0001:
                    op_or_vx_vy(system, &op_code);
                    break;

                case 0x0002:
                    op_and_vx_vy(system, &op_code);
                    break;

                case 0x0003:
                    op_xor_vx_vy(system, &op_code);
                    break;

                case 0x0004:
                    op_add_vx_vy(system, &op_code);
                    break;

                case 0x0005:
                    op_sub_vx_vy(system, &op_code);
                    break;

                case 0x0006:
                    op_shr_vx_vy(system, &op_code);
                    break;

                case 0x0007:
                    op_subn_vx_vy(system, &op_code);
                    break;

                case 0x000E:
                    op_shl_vx_vy(system, &op_code);
                    break;

                default:
                    printf("[FAILED] Unknown op code: 0x%X\n", op_code);
                    break;
            }

        // 9XY0: Skip next instruction when Vx != Vy
        case 0x9000:
            op_sne_vx_vy(system, &op_code);
            break;

        // ANNN: Set index register I
        case 0xA000:
            op_ld_i(system, &op_code);
            break;
        
        // BNNN: Jump to location NNN + V0
        case 0xB000:
            op_jp_v0(system, &op_code);
            break;

        // CXKK: Set Vx equal to a random byte AND KK
        case 0xC000:
            op_rnd_vx(system, &op_code);
            break;

        // DXYN: Display/draw
        case 0xD000:
            op_drw(system, &op_code);
            break;

        case 0xE000:
            switch (op_code & 0x00FF) {
                // EX9E: Skip next insturction if key with the valu eof Vx is pressed
                case 0x09E:
                    op_sne_vx(system, &op_code);
                    break;

                // EXA1: Skip next instruction if key with the value of Vx is not pressed
                case 0x00A1:
                    op_sknp_vx(system, &op_code);
                    break;
            }
            break;

        case 0xF000:
            switch (op_code & 0x00FF) {
                case 0x0007:
                    break;

                case 0x000A:
                    break;

                case 0x0015:
                    break;

                case 0x0018:
                    break;
                
                case 0x001E:
                    break;

                case 0x0029:
                    break;

                case 0x0033:
                    break;

                case 0x0055:
                    break;

                case 0x0065:
                    break;
            }
            break;

        // ERROR
        default:
            printf("Not implemented yet...");
            system->pc += 2;
            break;
    }
}