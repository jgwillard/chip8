#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdint.h>

#define FONT_START 0x050
#define PROGRAM_START 0x200

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define MEMORY_SIZE 4096
#define NUM_REGISTERS 16
#define STACK_SIZE 16
#define KEYPAD_SIZE 16
#define FONT_SIZE_BYTES 5

typedef struct Chip8Quirks {
  bool shift_uses_vx;
  bool load_store_increment_i;
  // logic opcodes reset VF to 0
  bool logic_resets_vf;
  // throttle draw opcode to one execution per frame
  bool draw_waits_for_vblank;
  // sprites get clipped at edge of display (no wrap)
  bool clip_sprites;
} Chip8Quirks;

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
  bool draw_flag;
  bool FX0A_waiting;
  uint8_t FX0A_key;
  uint8_t FX0A_reg;
  bool draw_permitted;
  const Chip8Quirks *quirks;
} Chip8;

typedef enum Chip8EventType { CHIP8_KEY_DOWN, CHIP8_KEY_UP } Chip8EventType;

typedef void (*chip8_draw_callback)(void *userdata);
typedef bool (*chip8_event_callback)(Chip8 *chip);
typedef uint64_t (*chip8_time_func)(void);
typedef void (*chip8_sleep_func)(uint32_t ms);

void chip8_init(Chip8 *chip, double clock_speed, bool debug,
                const Chip8Quirks *quirks);

int chip8_load_rom(Chip8 *chip, const char *filename);

void chip8_run(Chip8 *chip, chip8_draw_callback draw,
               chip8_event_callback handle_events, chip8_time_func current_time,
               chip8_sleep_func sleep, void *userdata);

void chip8_cycle(Chip8 *chip);

void chip8_key_event(Chip8 *chip, uint8_t key, Chip8EventType event_type);

void chip8_update_timers(Chip8 *chip);

uint16_t chip8_fetch(Chip8 *chip);

void chip8_decode_execute(Chip8 *chip, uint16_t opcode);

#endif
