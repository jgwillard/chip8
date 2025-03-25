#include "opcodes.h"

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
void op_00E0(Chip8 *chip, uint16_t opcode) { return; }

/**
 * return from a subroutine
 */
void op_00EE(Chip8 *chip, uint16_t opcode) { return; }

/**
 * jump to address NNN
 */
void op_1NNN(Chip8 *chip, uint16_t opcode) { return; }

/**
 * execute subroutine starting at address NNN
 */
void op_2NNN(Chip8 *chip, uint16_t opcode) { return; }

/**
 * if VX != n then execute the following instruction, else skip
 */
void op_3XNN(Chip8 *chip, uint16_t opcode) { return; }

/**
 * if VX == n then execute the following instruction, else skip
 */
void op_4XNN(Chip8 *chip, uint16_t opcode) { return; }

/**
 * if VX != VY then execute the following instruction, else skip
 */
void op_5XY0(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX := NN (store NN in register VX)
 */
void op_6XNN(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX += NN (add value NN to register VX)
 */
void op_7XNN(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX := VY (store value of VY in VX)
 */
void op_8XY0(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX |= VY (set VX to VX | VY)
 */
void op_8XY1(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX &= VY (set VX to VX & VY)
 */
void op_8XY2(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX ^= VY (set VX to VX ^ VY)
 */
void op_8XY3(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX += VY (set VX to VX + VY, set VF to 1 if carry occurs, else 0)
 */
void op_8XY4(Chip8 *chip, uint16_t opcode) { return; }

/**
 * VX -= VY (set VX to VX - VY, set VF to 0 if borrow occurs, else 1)
 */
void op_8XY5(Chip8 *chip, uint16_t opcode) { return; }

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
void op_9XY0(Chip8 *chip, uint16_t opcode) { return; }

/**
 * I := NNN (store memory address NNN to register I)
 */
void op_ANNN(Chip8 *chip, uint16_t opcode) { return; }

/**
 * jump to address NNN + V0
 */
void op_BNNN(Chip8 *chip, uint16_t opcode) { return; }

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
void op_DXYN(Chip8 *chip, uint16_t opcode) { return; }

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
void op_FX1E(Chip8 *chip, uint16_t opcode) { return; }

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
