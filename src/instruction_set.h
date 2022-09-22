#ifndef INSTRUCTION_SET_H_
#define INSTRUCTION_SET_H_

#include "chip8.h"

#include <stdint.h>

void op_cls(Chip8* system, uint16_t* op_code);
void op_ret(Chip8* system, uint16_t* op_code);
void op_jp(Chip8* system, uint16_t* op_code);
void op_call(Chip8* system, uint16_t* op_code);
void op_se(Chip8* system, uint16_t* op_code);
void op_sne(Chip8* system, uint16_t* op_code);
void op_se_compare(Chip8* system, uint16_t* op_code);
void op_ld(Chip8* system, uint16_t* op_code);
void op_add(Chip8* system, uint16_t *op_code);
void op_ld_i(Chip8* system, uint16_t* op_code);
void op_drw(Chip8* system, uint16_t* op_code);


#endif