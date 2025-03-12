#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

typedef struct Chip8 {
  uint8_t display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
} Chip8;

void chip8_init(Chip8* chip);

#endif

