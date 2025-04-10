#include "chip8.h"
#include "opcodes.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chip8_init(Chip8 *chip) {
  memset(chip, 0, sizeof(Chip8));
  chip->PC = PROGRAM_START;
}

int chip8_load_rom(Chip8 *chip, const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open ROM");
    return 1;
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  if (size > (MEMORY_SIZE - PROGRAM_START)) {
    perror("ROM too large");
    fclose(file);
    return 1;
  }

  fread(&chip->memory[PROGRAM_START], size, 1, file);
  fclose(file);
  return 0;
}

void chip8_run(Chip8 *chip, chip8_draw_callback draw,
               chip8_event_callback handle_events,
               chip8_time_func get_current_time, void *userdata) {

  uint64_t last_time = get_current_time();

  while (handle_events(chip->keypad)) {
    uint64_t current_time = get_current_time();
    uint64_t elapsed_time = current_time - last_time;

    if (elapsed_time >= (1000 / 60)) {
      last_time = current_time;

      chip8_cycle(chip);

      if (draw) {
        draw(userdata);
      }
    }
  }
}

void chip8_cycle(Chip8 *chip) {
  uint16_t opcode = chip8_fetch(chip);
  chip8_decode_execute(chip, opcode);
}

uint16_t chip8_fetch(Chip8 *chip) {
  return chip->memory[chip->PC] << 8 | chip->memory[chip->PC + 1];
}

void chip8_decode_execute(Chip8 *chip, uint16_t opcode) {
  uint8_t first_nibble = opcode >> 12;
  uint8_t last_nibble = opcode & 0x000F;
  uint8_t last_byte = opcode & 0x00FF;
  OpcodeHandler handler;

  switch (first_nibble) {
  case 0x0:
    handler = opcode_0XXX_table[last_byte];
    if (handler)
      handler(chip, opcode);
    break;

  case 0x8:
    handler = opcode_8XYN_table[last_nibble];
    if (handler)
      handler(chip, opcode);
    break;

  case 0xE:
    handler = opcode_EXXX_table[last_byte];
    if (handler)
      handler(chip, opcode);
    break;

  case 0xF:
    handler = opcode_FXXX_table[last_byte];
    if (handler)
      handler(chip, opcode);
    break;

  default:
    handler = opcode_main_table[first_nibble];
    if (handler)
      handler(chip, opcode);
    break;
  }
}
