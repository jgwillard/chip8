#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define PROGRAM_START 0x200

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define MEMORY_SIZE 4096
#define NUM_REGISTERS 16
#define STACK_SIZE 16
#define KEYPAD_SIZE 16

typedef struct Chip8 {
  uint8_t display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
  uint8_t keypad[KEYPAD_SIZE];
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint8_t memory[MEMORY_SIZE];
  uint8_t V[NUM_REGISTERS];
  uint16_t I;
  uint16_t PC;
  uint16_t stack[STACK_SIZE];
  uint8_t SP;
} Chip8;

typedef void (*chip8_draw_callback)(void *userdata);
typedef void (*chip8_input_callback)(uint8_t *keypad);

void chip8_init(Chip8 *chip);
void chip8_rom_load(Chip8 *chip, const char *filename);
void chip8_run(Chip8 *chip, chip8_draw_callback draw,
               chip8_input_callback input, void *userdata);
void chip8_cycle(Chip8 *chip);
uint16_t chip8_fetch(Chip8 *chip);
void chip8_decode_execute(Chip8 *chip, uint16_t opcode);

#endif
