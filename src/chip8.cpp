#include "chip8.hpp"
#include "instruction_set.hpp"

#include <fstream>
#include <iostream>

Chip8::Chip8(bool debug, uint16_t delayTimer, Display* io)
    : pc(0x200),
    I(0),
    sp(0),
    dt(0),
    st(0),
    drawFlag(0),
    soundFlag(0),
    debugFlag(debug),
    stepFlag(0),
    display(),
    memory(),
    V(),
    stack(),
    io(io),
    delayTimer(delayTimer)
{
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
        this->memory[i] = fontset[i];
    }

    // Initialize keypad
    for (int i = 0; i < KEYPAD_SIZE; i++) {
        this->keypad[i] = 0;
    }
}

void Chip8::loadRom(char* filename)
{
    std::streampos size;
    char* memblock;

    std::ifstream rom (filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (rom.is_open()) {
        size = rom.tellg();
        memblock = new char[size];
        rom.seekg(0, std::ios::beg);
        rom.read(memblock, size);
        rom.close();
    }

    for (int i = 0; i < size; i++) {
        this->memory[i+0x200] = memblock[i];
    }

    // Cleanup
    delete[] memblock;
}

void Chip8::emulateCycle(void)
{
    this->drawFlag = 0;
    this->soundFlag = 0;

    uint16_t opCode = this->memory[this->pc] << 8 | this->memory[this->pc+1];
    
    if (this->debugFlag) {
        printf("pc: 0x%X\n", this->pc);
        printf("opcode: 0x%X\n", opCode);
    }

    // Increment PC
    this->pc += 2;
    
    // Mask to get "first nibble"
    switch (opCode & 0xF000) {
        case 0x0000:
            switch (opCode & 0x00FF) {
                // 00E0: Clear screen
                case 0x00E0:
                    op_cls(this, &opCode);
                    break;

                // OOEE: Return
                case 0x00EE:
                    op_ret(this, &opCode);
                    break;
                
                default:
                    printf("[FAILED] Unknown opcode: 0x%X\n", opCode);
                    break;
            }
            break;
        
        // 1NNN: Jump to NNN
        case 0x1000:
            op_jp(this, &opCode);
            break;

        // 2NNN: Call
        case 0x2000:
            op_call(this, &opCode);
            break;

        // 3XNN: Skip next if x == NN
        case 0x3000:
            op_se(this, &opCode);
            break;

        // 4XNN: Skip next if x != NN
        case 0x4000:
            op_sne(this, &opCode);
            break;

        // 5XY0: Skip next insturction if Vx == Vy
        case 0x5000:
            op_se_compare(this, &opCode);
            break;

        // 6XNN: Set Vx equal to NN
        case 0x6000:
            op_ld_vx(this, &opCode);
            break;

        // 7XNN: Add NN to X
        case 0x7000:
            op_add(this, &opCode);
            break;

        // 8000 Cases
        case 0x8000:
            switch (opCode & 0x000F) {
                case 0x0000:
                    op_ld_vx_vy(this, &opCode);
                    break;

                case 0x0001:
                    op_or_vx_vy(this, &opCode);
                    break;

                case 0x0002:
                    op_and_vx_vy(this, &opCode);
                    break;

                case 0x0003:
                    op_xor_vx_vy(this, &opCode);
                    break;

                case 0x0004:
                    op_add_vx_vy(this, &opCode);
                    break;

                case 0x0005:
                    op_sub_vx_vy(this, &opCode);
                    break;

                case 0x0006:
                    op_shr_vx_vy(this, &opCode);
                    break;

                case 0x0007:
                    op_subn_vx_vy(this, &opCode);
                    break;

                case 0x000E:
                    op_shl_vx_vy(this, &opCode);
                    break;

                default:
                    printf("[FAILED] Unknown op code: 0x%X\n", opCode);
                    break;
            }
            break;

        // 9XY0: Skip next instruction when Vx != Vy
        case 0x9000:
            op_sne_vx_vy(this, &opCode);
            break;

        // ANNN: Set index register I
        case 0xA000:
            op_ld_i(this, &opCode);
            break;
        
        // BNNN: Jump to location NNN + V0
        case 0xB000:
            op_jp_v0(this, &opCode);
            break;

        // CXKK: Set Vx equal to a random byte AND KK
        case 0xC000:
            op_rnd_vx(this, &opCode);
            break;

        // DXYN: Display/draw
        case 0xD000:
            op_drw(this, &opCode);
            break;

        case 0xE000:
            switch (opCode & 0x00FF) {
                // EX9E: Skip next insturction if key with the valu eof Vx is pressed
                case 0x09E:
                    op_sne_vx(this, &opCode);
                    break;

                // EXA1: Skip next instruction if key with the value of Vx is not pressed
                case 0x00A1:
                    op_sknp_vx(this, &opCode);
                    break;
            }
            break;

        case 0xF000:
            switch (opCode & 0x00FF) {
                case 0x0007:
                    // FX07: LD Vx, DT - the value of DT is placed into Vx
                    op_ld_vx_dt(this, &opCode);
                    break;

                case 0x000A:
                    // FX0A: LD Vx, K - the value of K is placed into Vx
                    op_ld_vx_k(this, &opCode);
                    break;

                case 0x0015:
                    // FX15: LD DT, Vx - set delay timer equal to Vx
                    op_ld_dt_vx(this, &opCode);
                    break;

                case 0x0018:
                    // FX18: LD ST, Vx - set sound timer = Vx
                    op_ld_st_vx(this, &opCode);
                    break;
                
                case 0x001E:
                    // FX1E - ADD I, Vx - set I = I + Vx
                    op_add_i_vx(this, &opCode);
                    break;

                case 0x0029:
                    // FX29: LD F, Vx - set I = location of sprite for digit Vx
                    op_ld_f_vx(this, &opCode);
                    break;

                case 0x0033:
                    // FX33: LD B, Vx - store BCD representation of Vx in memory locations\
                    // I, I+1, I+2
                    op_ld_b_vx(this, &opCode);
                    break;

                case 0x0055:
                    // FX55: LD [I], Vx - store registers V0 -> Vx in memory starting at
                    // location I
                    op_ld_i_vx(this, &opCode);
                    break;

                case 0x0065:
                    // FX65: LD Vx, [I] - read registers V0 -> Vx from memory starting at
                    // location I
                    op_ld_vx_i(this, &opCode);
                    break;
            }
            break;

        // ERROR
        default:
            printf("Opcode not implemented");
            this->pc += 2;
            break;
    }

    if (this->dt > 0) this->dt--;
    if (this->st > 0) this->st--;
}