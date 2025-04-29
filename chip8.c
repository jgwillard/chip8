#include "chip8.h"
#include "opcodes.h"
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool DEBUG = true;

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

  double debug_timer = 0.0;
  uint64_t instruction_counter = 0;
  uint64_t frame_counter = 0;

  bool running = true;
  const double cycles_per_second = 700.0;
  const double frames_per_second = 60.0;

  const double milliseconds_per_cycle = 1000.0 / cycles_per_second;
  const double milliseconds_per_frame = 1000.0 / frames_per_second;

  double cycle_accumulator = 0.0;
  double frame_accumulator = 0.0;

  double last_time = get_current_time();

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
      instruction_counter++;
      cycle_accumulator -= milliseconds_per_cycle;
    }

    // run a frame update if enough time has elapsed (update timers and
    // draw screen if display has been updated)
    while (frame_accumulator >= milliseconds_per_frame) {
      chip8_update_timers(chip);
      frame_accumulator -= milliseconds_per_frame;
      frame_counter++;

      if (draw && chip->draw_flag) {
        draw(userdata);
        chip->draw_flag = false;
      }
    }

    // calculate the amount of time we can safely sleep (so the program
    // does not use 100% and waste iterations) before we need to
    // run another loop
    double next_cycle_due = milliseconds_per_cycle - cycle_accumulator;
    double next_timer_due = milliseconds_per_frame - frame_accumulator;
    uint32_t sleep_time =
        (uint32_t)fmax(1.0, fmin(next_cycle_due, next_timer_due));

    sleep_for_milliseconds(sleep_time);

    if (DEBUG) {
      // print execution speed and FPS info
      debug_timer += elapsed_time;
      if (debug_timer >= 1000.0) {
        printf("Executed %" PRIu64 " instructions in last second\n",
               instruction_counter);
        instruction_counter = 0;
        printf("Rendered %" PRIu64 " frames in last second\n", frame_counter);
        frame_counter = 0;
        debug_timer = 0.0;
      }
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
