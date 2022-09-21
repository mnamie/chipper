#ifndef INSTRUCTION_SET_H_
#define INSTRUCTION_SET_H_

#include "chip8.h"

#include <stdint.h>

void op_clear_screen(Chip8* system, uint16_t* op_code);

#endif