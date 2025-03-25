#ifndef OPCODES_H
#define OPCODES_H

#include "chip8.h"
#include <stdint.h>

typedef void (*OpcodeHandler)(Chip8 *chip, uint16_t opcode);

extern OpcodeHandler opcode_0XXX_table[0x100];
extern OpcodeHandler opcode_8XYN_table[0x10];
extern OpcodeHandler opcode_EXXX_table[0x10];
extern OpcodeHandler opcode_FXXX_table[0x10];
extern OpcodeHandler opcode_main_table[0x10];

void op_00E0(Chip8 *chip, uint16_t opcode);
void op_00EE(Chip8 *chip, uint16_t opcode);

void op_1NNN(Chip8 *chip, uint16_t opcode);
void op_2NNN(Chip8 *chip, uint16_t opcode);
void op_3XNN(Chip8 *chip, uint16_t opcode);
void op_4XNN(Chip8 *chip, uint16_t opcode);
void op_5XY0(Chip8 *chip, uint16_t opcode);
void op_6XNN(Chip8 *chip, uint16_t opcode);
void op_7XNN(Chip8 *chip, uint16_t opcode);

void op_8XY0(Chip8 *chip, uint16_t opcode);
void op_8XY1(Chip8 *chip, uint16_t opcode);
void op_8XY2(Chip8 *chip, uint16_t opcode);
void op_8XY3(Chip8 *chip, uint16_t opcode);
void op_8XY4(Chip8 *chip, uint16_t opcode);
void op_8XY5(Chip8 *chip, uint16_t opcode);
void op_8XY6(Chip8 *chip, uint16_t opcode);
void op_8XY7(Chip8 *chip, uint16_t opcode);
void op_8XYE(Chip8 *chip, uint16_t opcode);

void op_9XY0(Chip8 *chip, uint16_t opcode);
void op_ANNN(Chip8 *chip, uint16_t opcode);
void op_BNNN(Chip8 *chip, uint16_t opcode);
void op_CXNN(Chip8 *chip, uint16_t opcode);
void op_DXYN(Chip8 *chip, uint16_t opcode);

void op_EX9E(Chip8 *chip, uint16_t opcode);
void op_EXA1(Chip8 *chip, uint16_t opcode);

void op_FX07(Chip8 *chip, uint16_t opcode);
void op_FX0A(Chip8 *chip, uint16_t opcode);
void op_FX15(Chip8 *chip, uint16_t opcode);
void op_FX18(Chip8 *chip, uint16_t opcode);
void op_FX1E(Chip8 *chip, uint16_t opcode);
void op_FX29(Chip8 *chip, uint16_t opcode);
void op_FX33(Chip8 *chip, uint16_t opcode);
void op_FX55(Chip8 *chip, uint16_t opcode);
void op_FX65(Chip8 *chip, uint16_t opcode);

#endif
