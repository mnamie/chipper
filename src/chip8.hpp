#ifndef _CHIP_HPP_
#define _CHIP_HPP_

#include "io.hpp"

#include <cstdint>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define KEYPAD_SIZE 16

class Display;

class Chip8 
{
public:
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
    bool drawFlag;
    bool soundFlag;
    bool debugFlag;
    bool stepFlag;
    uint16_t delayTimer;

    Display* io;

    Chip8(bool, uint16_t, Display*);
    void loadRom(char*);
    void emulateCycle(void);
};

#endif
