#ifndef CHIP_H_
#define CHIP_H_

#include <stdint.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define KEYPAD_SIZE 16

typedef struct Chip8 {
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[16];
    uint8_t keypad[KEYPAD_SIZE];
    uint8_t display[SCREEN_HEIGHT][SCREEN_WIDTH];
    uint32_t buffer[SCREEN_HEIGHT * SCREEN_WIDTH];
    uint8_t dt;
    uint8_t st;
    uint8_t draw_flag;
    uint8_t sound_flag;
    uint8_t debug_flag;
} Chip8;

void init_chip8(Chip8* system);
int load_rom(Chip8* system, char* filename);
void emulate_cycle(Chip8* system);

#endif