#include "chip8.h"
#include "opcodes.h"
#include <math.h>
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
               chip8_time_func get_current_time,
               chip8_sleep_func sleep_for_milliseconds, void *userdata) {

  bool running = true;
  const uint32_t cycles_per_second = 700.0;
  const uint32_t frames_per_second = 60.0;

  const uint32_t milliseconds_per_cycle = 1000.0 / cycles_per_second;
  const uint32_t milliseconds_per_frame = 1000.0 / frames_per_second;

  uint32_t cycle_accumulator = 0.0;
  uint32_t frame_accumulator = 0.0;

  uint32_t last_time = get_current_time();

  while (running) {
    // for each iteration, update keypad data
    running = handle_events(chip->keypad);

    // get amount of time since last iteration
    double current_time = get_current_time();
    double elapsed_time = current_time - last_time;
    last_time = current_time;

    // accumulators keep amount of time that has passed since last
    // iteration
    cycle_accumulator += elapsed_time;
    frame_accumulator += elapsed_time;

    // run the number of CPU cycles that should have run since the last
    // iteration (this can vary depending on the host)
    while (cycle_accumulator >= milliseconds_per_cycle) {
      chip8_cycle(chip);
      cycle_accumulator -= milliseconds_per_cycle;
    }

    // run a frame update if enough time has elapsed (update timers and
    // draw screen if display has been updated)
    if (frame_accumulator >= milliseconds_per_frame) {
      chip8_update_timers(chip);
      frame_accumulator -= milliseconds_per_frame;

      if (draw && chip->draw_flag) {
        draw(userdata);
        chip->draw_flag = false;
      }
    }

    uint64_t next_cycle_due = milliseconds_per_cycle - cycle_accumulator;
    uint64_t next_timer_due = milliseconds_per_frame - frame_accumulator;
    uint64_t sleep_time = fmin(next_cycle_due, next_timer_due);

    if (sleep_time > 0.5) {
      sleep_for_milliseconds(sleep_time);
    } else {
      sleep_for_milliseconds(0.0);
    }
  }
}

void chip8_update_timers(Chip8 *chip) {
  if (chip->delay_timer > 0)
    chip->delay_timer -= 1;
  if (chip->sound_timer > 0)
    chip->sound_timer -= 1;
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
