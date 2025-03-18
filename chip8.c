#include "chip8.h"
#include "SDL_timer.h"
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

void chip8_load_rom(Chip8 *chip, const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open ROM");
    return;
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  if (size > (MEMORY_SIZE - PROGRAM_START)) {
    perror("ROM too large");
    fclose(file);
    return;
  }

  fread(&chip->memory[PROGRAM_START], size, 1, file);
  fclose(file);
}

void chip8_run(Chip8 *chip, chip8_draw_callback draw,
               chip8_input_callback input, void *userdata) {
  uint64_t last_time = SDL_GetTicks();

  while (true) {
    uint32_t current_time = SDL_GetTicks();
    uint32_t elapsed_time = current_time - last_time;

    if (elapsed_time >= (1000 / 60)) {
      last_time = current_time;

      if (input) {
        input(chip->keypad);
      }

      chip8_cycle(chip);

      if (draw) {
        draw(userdata);
      }
    }
  }
}

void chip8_cycle(Chip8 *chip) {
  // fetch 16-bit opcode
  uint16_t opcode = chip->memory[chip->PC] << 8 | chip->memory[chip->PC + 1];

  // TODO decode
}
