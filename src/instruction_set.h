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
void op_ld_vx(Chip8* system, uint16_t* op_code);
void op_add(Chip8* system, uint16_t *op_code);
void op_ld_vx_vy(Chip8* system, uint16_t* op_code);
void op_or_vx_vy(Chip8* system, uint16_t* op_code);
void op_and_vx_vy(Chip8* system, uint16_t* op_code);
void op_xor_vx_vy(Chip8* system, uint16_t* op_code);
void op_add_vx_vy(Chip8* system, uint16_t* op_code);
void op_sub_vx_vy(Chip8* system, uint16_t* op_code);
void op_shr_vx_vy(Chip8* system, uint16_t* op_code);
void op_subn_vx_vy(Chip8* system, uint16_t* op_code);
void op_shl_vx_vy(Chip8* system, uint16_t* op_code);
void op_ld_i(Chip8* system, uint16_t* op_code);
void op_drw(Chip8* system, uint16_t* op_code);


#endif