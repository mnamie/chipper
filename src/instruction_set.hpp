#ifndef _INSTRUCTION_SET_HPP_
#define _INSTRUCTION_SET_HPP_

#include "chip8.hpp"
#include "io.hpp"

#include <stdint.h>

void op_cls(Chip8 *system, uint16_t *op_code);
void op_ret(Chip8 *system, uint16_t *op_code);
void op_jp(Chip8 *system, uint16_t *op_code);
void op_call(Chip8 *system, uint16_t *op_code);
void op_se(Chip8 *system, uint16_t *op_code);
void op_sne(Chip8 *system, uint16_t *op_code);
void op_se_compare(Chip8 *system, uint16_t *op_code);
void op_ld_vx(Chip8 *system, uint16_t *op_code);
void op_add(Chip8 *system, uint16_t *op_code);
void op_ld_vx_vy(Chip8 *system, uint16_t *op_code);
void op_or_vx_vy(Chip8 *system, uint16_t *op_code);
void op_and_vx_vy(Chip8 *system, uint16_t *op_code);
void op_xor_vx_vy(Chip8 *system, uint16_t *op_code);
void op_add_vx_vy(Chip8 *system, uint16_t *op_code);
void op_sub_vx_vy(Chip8 *system, uint16_t *op_code);
void op_shr_vx_vy(Chip8 *system, uint16_t *op_code);
void op_subn_vx_vy(Chip8 *system, uint16_t *op_code);
void op_shl_vx_vy(Chip8 *system, uint16_t *op_code);
void op_sne_vx_vy(Chip8 *system, uint16_t *op_code);
void op_ld_i(Chip8 *system, uint16_t *op_code);
void op_jp_v0(Chip8 *system, uint16_t *op_code);
void op_rnd_vx(Chip8 *system, uint16_t *op_code);
void op_drw(Chip8 *system, uint16_t *op_code);
void op_sne_vx(Chip8 *system, uint16_t *op_code);
void op_sknp_vx(Chip8 *system, uint16_t *op_code);
void op_ld_vx_dt(Chip8 *system, uint16_t *op_code);
void op_ld_vx_k(Chip8 *system, uint16_t *op_code);
void op_ld_dt_vx(Chip8 *system, uint16_t *op_code);
void op_ld_st_vx(Chip8 *system, uint16_t *op_code);
void op_add_i_vx(Chip8 *system, uint16_t *op_code);
void op_ld_f_vx(Chip8 *system, uint16_t *op_code);
void op_ld_b_vx(Chip8 *system, uint16_t *op_code);
void op_ld_i_vx(Chip8 *system, uint16_t *op_code);
void op_ld_vx_i(Chip8 *system, uint16_t *op_code);

#endif