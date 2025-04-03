#include "opcodes.h"
#include "chip8.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
 * HELPER FUNCTIONS
 */

/**
 * increment program counter
 */
void _inc_pc(Chip8 *chip) { chip->PC += 2; }

/**
 * OPCODES
 *
 * one function corresponding to each chip8 opcode is implemented below
 * see https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set
 * or https://johnearnest.github.io/Octo/docs/chip8ref.pdf
 */

/**
 * clear the screen
 */
void op_00E0(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  memset(chip->display, 0, sizeof(chip->display));
}

/**
 * return from a subroutine
 */
void op_00EE(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  chip->PC = chip->stack[chip->SP];
  chip->SP--;
}

/**
 * jump to address NNN
 */
void op_1NNN(Chip8 *chip, uint16_t opcode) { chip->PC = opcode & 0x0FFF; }

/**
 * execute subroutine starting at address NNN
 */
void op_2NNN(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  chip->stack[chip->SP] = chip->PC;
  chip->PC = opcode & 0x0FFF;
  chip->SP++;
}

/**
 * if VX != n then execute the following instruction, else skip
 */
void op_3XNN(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t n = opcode & 0x00FF;
  if (chip->V[x] == n) {
    _inc_pc(chip);
  }
}

/**
 * if VX == n then execute the following instruction, else skip
 */
void op_4XNN(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = opcode & 0x0F00 >> 8;
  uint8_t n = opcode & 0x00FF;
  if (chip->V[x] != n) {
    _inc_pc(chip);
  }
}

/**
 * if VX != VY then execute the following instruction, else skip
 */
void op_5XY0(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  if (x == y) {
    _inc_pc(chip);
  }
}

/**
 * VX := NN (store NN in register VX)
 */
void op_6XNN(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t n = opcode & 0x00FF;
  chip->V[x] = n;
}

/**
 * VX += NN (add value NN to register VX)
 */
void op_7XNN(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t n = opcode & 0x00FF;
  chip->V[x] += n;
}

/**
 * VX := VY (store value of VY in VX)
 */
void op_8XY0(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  chip->V[x] = chip->V[y];
}

/**
 * VX |= VY (set VX to VX | VY)
 */
void op_8XY1(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  chip->V[x] |= chip->V[y];
}

/**
 * VX &= VY (set VX to VX & VY)
 */
void op_8XY2(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  chip->V[x] &= chip->V[y];
}

/**
 * VX ^= VY (set VX to VX ^ VY)
 */
void op_8XY3(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  chip->V[x] ^= chip->V[y];
}

/**
 * VX += VY (set VX to VX + VY, set VF to 1 if carry occurs, else 0)
 */
void op_8XY4(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  chip->V[x] += chip->V[y];
}

/**
 * VX -= VY (set VX to VX - VY, set VF to 0 if borrow occurs, else 1)
 */
void op_8XY5(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  chip->V[x] -= chip->V[y];
}

/**
 * set VX to VY >> 1 (shifted right one bit), set VF to least
 * significant (rightmost) bit prior to the shift
 */
void op_8XY6(Chip8 *chip, uint16_t opcode) { return; }

/**
 * set VX to VY - VX, set VF to 0 if borrow occurs, else 1)
 */
void op_8XY7(Chip8 *chip, uint16_t opcode) { return; }

/**
 * set VX to VY << 1 (shifted left one bit), set VF to most
 * significant (leftmost) bit prior to the shift
 */
void op_8XYE(Chip8 *chip, uint16_t opcode) { return; }

/**
 * if VX == VY then execute the following instruction, else skip
 */
void op_9XY0(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  if (chip->V[x] != chip->V[y]) {
    _inc_pc(chip);
  }
}

/**
 * I := NNN (store memory address NNN to register I)
 */
void op_ANNN(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  chip->I = opcode & 0x0FFF;
}

/**
 * jump to address NNN + V0
 */
void op_BNNN(Chip8 *chip, uint16_t opcode) {
  uint16_t n = opcode & 0x0FFF;
  chip->PC = n + chip->V[0];
}

/**
 * VX := RANDOM && NN (set VX to a random 8-bit (0-255) number with a
 * mask of NN)
 */
void op_CXNN(Chip8 *chip, uint16_t opcode) { return; }

/**
 * draw a sprite at position (VX, VY) with N bytes of sprite data
 * starting at the address stored in register I
 * set VF to 1 on collision (i.e. any pixels change from 1 to 0)
 */
void op_DXYN(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  chip->V[0xF] = 0;
  uint8_t x_pointer = (opcode & 0x0F00) >> 8;
  uint8_t y_pointer = (opcode & 0x00F0) >> 4;
  uint8_t x = chip->V[x_pointer] % DISPLAY_WIDTH;
  uint8_t y = chip->V[y_pointer] % DISPLAY_HEIGHT;
  uint8_t n = opcode & 0x000F;
  for (int i = 0; i < n; i++) {
    uint8_t byte = chip->memory[chip->I + i];
    for (int j = 0; j < 8; j++) {
      int bit = (1 << (7 - j)) & byte;
      chip->display[(y + i) * DISPLAY_HEIGHT + x + j] ^= bit;
      if (chip->display[(y + i) * DISPLAY_HEIGHT + x + j] == 0 && bit == 1) {
        chip->V[0xF] = 1;
      }
    }
  }
}

/**
 * if the key corresponding to the value in VX is not pressed, execute
 * the following instruction; otherwise skip
 */
void op_EX9E(Chip8 *chip, uint16_t opcode) { return; }

/**
 * if the key corresponding to the value in VX is pressed, execute
 * the following instruction; otherwise skip
 */
void op_EXA1(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX := delay (store current value of delay timer in VX)
 */
void op_FX07(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX := key (wait for keypress and store value in VX)
 */
void op_FX0A(Chip8 *chip, uint16_t opcode) { return; }

/**
 * delay := VX (set the delay timer to the value of register VX)
 */
void op_FX15(Chip8 *chip, uint16_t opcode) { return; }

/**
 * sound := VX (set the sound timer to the value of register VX)
 */
void op_FX18(Chip8 *chip, uint16_t opcode) { return; }

/**
 * I += VX (add value stored in register VX to register I)
 */
void op_FX1E(Chip8 *chip, uint16_t opcode) {
  _inc_pc(chip);
  uint8_t x = (opcode & 0x0F00) >> 8;
  chip->I += chip->V[x];
}

/**
 * I := hex VX (set I to the address corresponding to the hexadecimal
 * digit stored in register VX)
 */
void op_FX29(Chip8 *chip, uint16_t opcode) { return; }

/**
 * bcd VX (store the binary-coded decimal equivalent of the value stored
 * in VX at addresses I, I + 1, and I + 2)
 */
void op_FX33(Chip8 *chip, uint16_t opcode) { return; }

/**
 * save VX (store the values of registers V0-VX in memory starting at
 * address I; I := I + X + 1)
 */
void op_FX55(Chip8 *chip, uint16_t opcode) { return; }

/**
 * load VX (set values of regsiters V0-VX with the values in memory
 * starting at address I; I := I + X + 1)
 */
void op_FX65(Chip8 *chip, uint16_t opcode) { return; }

/**
 * FUNCTION POINTER TABLES
 */
OpcodeHandler opcode_0XXX_table[0x100] = {
    [0xE0] = op_00E0,
    [0xEE] = op_00EE,
};

OpcodeHandler opcode_8XYN_table[0x10] = {
    [0x0] = op_8XY0, [0x1] = op_8XY1, [0x2] = op_8XY2,
    [0x3] = op_8XY3, [0x4] = op_8XY4, [0x5] = op_8XY5,
    [0x6] = op_8XY6, [0x7] = op_8XY7, [0xE] = op_8XYE,
};

OpcodeHandler opcode_EXXX_table[0x100] = {
    [0x9E] = op_EX9E,
    [0xA1] = op_EXA1,
};

OpcodeHandler opcode_FXXX_table[0x100] = {
    [0x07] = op_FX07, [0x0A] = op_FX0A, [0x15] = op_FX15,
    [0x18] = op_FX18, [0x1E] = op_FX1E, [0x29] = op_FX29,
    [0x33] = op_FX33, [0x55] = op_FX55, [0x65] = op_FX65,
};

OpcodeHandler opcode_main_table[0x10] = {
    [0x1] = op_1NNN, [0x2] = op_2NNN, [0x3] = op_3XNN, [0x4] = op_4XNN,
    [0x5] = op_5XY0, [0x6] = op_6XNN, [0x7] = op_7XNN, [0x9] = op_9XY0,
    [0xA] = op_ANNN, [0xB] = op_BNNN, [0xC] = op_CXNN, [0xD] = op_DXYN};
