#include "instruction_set.h"
#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// 00E0: Clear screen
void op_cls(Chip8* system, uint16_t* op_code)
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

// 00EE: Return
void op_ret(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 00EE\n", *op_code); }
    system->pc = system->stack[system->sp];
    system->sp--;
    system->pc += 2;
}

// 1NNN: Jump to NNN
void op_jp(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 1NNN\n", *op_code); }
    system->pc = (*op_code & 0x0FFF);
}

// 2NNN: Call
void op_call(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 2NNN\n", *op_code); }
    system->sp++;
    system->stack[system->sp] = system->pc;
    system->pc = *op_code & 0x0FFF;
}

// 3XNN: Skip next instruction if Vx == NN
void op_se(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 3XNN\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    if (system->V[x] == (*op_code & 0x00FF)) {
        system->pc += 2;
    }
    system->pc += 2;
}

// 4XNN: Skip next if x != NN
void op_sne(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 4XNN\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    if (system->V[x] != (*op_code & 0x00FF)) {
        system->pc += 2;
    }
    system->pc += 2;
}

// 5XY0: Skip next insturction if Vx == Vy
void op_se_compare(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 5XYN\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    uint16_t y = (*op_code & 0x00F0) >> 4;
    if (system->V[x] == system->V[y]) {
        system->pc += 2;
    }
    system->pc += 2;
}

// 6XNN: Set Vx equal to NN
void op_ld_vx(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 6XNN\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    system->V[x] = (*op_code & 0x00FF);
    system->pc += 2;
}

// 7XNN: Add NN to X
void op_add(Chip8* system, uint16_t *op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 7XNN\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    system->V[x] = system->V[x] + (*op_code & 0x00FF);
    system->pc += 2;
}

// 8XY0: LD Vx, Vy
void op_ld_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 8XY0\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;
    system->V[x] = system->V[y];
    system->pc += 2;
}

// 8XY1: ADD Vx, Vy
void op_or_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 8XY1\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;
    system->V[x] = system->V[x] | system->V[y];
    system->pc += 2;
}

// 8XY2: AND Vx, Vy
void op_and_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 8XY2\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;
    system->V[x] = system->V[x] & system->V[y];
    system->pc += 2;
}

// 8XY3: AND Vx, Vy
void op_xor_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 8XY3\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;
    system->V[x] = system->V[x] ^ system->V[y];
    system->pc += 2;
}

// 8XY4: ADD Vx, Vy
void op_add_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 8XY4\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;
    system->V[0xF] = (system->V[x] + system->V[y] > 0xFF) ? 1 : 0;
    system->V[x] += system->V[y];
    system->pc += 2;
}

// 8XY5: SUB Vx, Vy
void op_sub_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 8XY5\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;
    system->V[0xF] = (system->V[x] > system->V[y]) ? 1 : 0;
    system->V[x] -= system->V[y];
    system->pc += 2;
}

// 8XY6: SHR Vx, Vy
void op_shr_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 8XY6\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;
    system->V[0xF] = system->V[x] & 0x1;
    system->V[x] = (system->V[y] >> 1);
    system->pc += 2;
}

// 8XY7: SUBN Vx, Vy
void op_subn_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 8XY7\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;
    system->V[0xF] = (system->V[y] > system->V[x]) ? 1 : 0;
    system->V[x] = (system->V[y] - system->V[x]);
    system->pc += 2;
}

// 8XYE: SHL Vx, Vy
void op_shl_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 8XYE\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;
    system->V[0xF] = (system->V[x] >> 7) & 0x1;
    system->V[x] = (system->V[x] << 1);
    system->pc += 2;
}

// 9XY0: SNE Vx, Vy - Skip next instruction if Vx != Vy
void op_sne_vx_vy(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: 9XY0\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    uint16_t y = (*op_code & 0x00F0) >> 4;
    if (system->V[x] != system->V[y]) {
        system->pc += 2;
    }
    system->pc += 2;
}

// ANNN: Set index register I
void op_ld_i(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: ANNN\n", *op_code); }
    system->I = (*op_code & 0x0FFF);
    system->pc += 2;
}

// BNNN: Jump to location NNN + V0
void op_jp_v0(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: BNNN\n", *op_code); }
    uint16_t nnn = *op_code & 0x0FFF;
    system->pc = system->V[0] + nnn;
}

// CXKK: Set Vx equal to a random byte AND KK
void op_rnd_vx(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: CXKK\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    uint16_t kk = (*op_code & 0x00FF);
    uint8_t random_num = rand() % 256;

    system->V[x] = random_num & kk;
    system->pc += 2;
}

// DXYN: Display/draw
void op_drw(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: DXYN\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int16_t y = (*op_code & 0x00F0) >> 4;

    uint16_t x_loc = system->V[x];
    uint16_t y_loc = system->V[y];
    uint16_t sprite_height = (*op_code & 0x000F);
    
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
}

// EX9E: Skip next insturction if key with the valu eof Vx is pressed
void op_sne_vx(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: EX9E\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    uint16_t key = system->V[x];
    if (system->keypad[key]) {
        system->pc += 2;
    }
    system->pc += 2;
}

// EXA1: Skip next instruction if key with the value of Vx is not pressed
void op_sknp_vx(Chip8* system, uint16_t* op_code)
{
    if (system->debug_flag == 1) { printf("[OK] 0x%X: EXA1\n", *op_code); }
    uint16_t x = (*op_code & 0x0F00) >> 8;
    uint16_t key = system->V[x];
    if (!system->keypad[key]) {
        system->pc += 2;
    }
    system->pc += 2;
}

// FX07: LD Vx, DT - the value of DT is placed into Vx
void op_ld_vx_dt(Chip8* system, uint16_t* op_code)
{
    uint16_t x = (*op_code & 0x0F00) >> 8;
    system->V[x] = system->dt;
    system->pc += 2;
}

// FX0A: LD Vx, K - the value of K is placed into Vx
void op_ld_vx_k(Chip8* system, uint16_t* op_code)
{
    uint16_t x = (*op_code & 0x0F00) >> 8;
    int flag = 0;

    for (int i = 0; i < sizeof(system->V); i++) {
        if (system->V[i]) {
            system->V[x] = i;
            flag = 1;
        }
    }
    if (flag == 0) {
        return;
    }
    system->pc += 2;
}

// FX15: LD DT, Vx - set delay timer equal to Vx
void op_d_dt_vx(Chip8* system, uint16_t* op_code)
{
    uint16_t x = (*op_code & 0x0F00) >> 8;
    system->dt = system->V[x];
    system->pc += 2;
}

// FX18: LD ST, Vx - set sound timer = Vx
void op_ld_st_vx(Chip8* system, uint16_t* op_code)
{
    uint16_t x = (*op_code & 0x0F00) >> 8;
    system->st = system->V[x];
    system->pc += 2;
}

// FX1E: ADD I, Vx - set I = I + Vx
void op_add_i_vx(Chip8* system, uint16_t* op_code)
{
    uint16_t x = (*op_code & 0x0F00) >> 8;
    system->I = system->I + system->V[x];
    system->pc += 2;
}

// FX29: LD F, Vx - set I = location of sprite for digit Vx
void op_ld_f_vx(Chip8* system, uint16_t* op_code)
{
    uint16_t x = (*op_code & 0x0F00) >> 8;
    uint16_t digit = system->V[x];
    system->I = 5 * digit;
    system->pc += 2;
}

// FX33: LD B, Vx - store BCD representation of Vx in memory locations\
// I, I+1, I+2
void op_ld_b_vx(Chip8* system, uint16_t* op_code)
{
    uint16_t x = (*op_code & 0x0F00) >> 8;
    uint16_t value = system->V[x];

    system->memory[system->I +2] = value % 10;
    value /= 10;

    system->memory[system->I + 1] = value % 10;
    value /= 10;

    system->memory[system->I] = value % 10;
    
    system->pc += 2;
}

// FX55: LD [I], Vx - store registers V0 -> Vx in memory starting at
// location I
void op_ld_i_vx(Chip8* system, uint16_t* op_code)
{
    uint16_t x = (*op_code & 0x0F00) >> 8;

    for (uint16_t i = 0; i < x; i++) {
        system->memory[system->I + i] = system->V[i];
    }
    
    system->pc += 2;
}

// FX65: LD Vx, [I] - read registers V0 -> Vx from memory starting at
// location I
void op_ld_vx_i(Chip8* system, uint16_t* op_code)
{
    uint16_t x = (*op_code & 0x0F00) >> 8;

    for (uint16_t i = 0; i < x; i++) {
        system->V[i] = system->memory[system->I + i];
    }

    system->pc += 2;
}