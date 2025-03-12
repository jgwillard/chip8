#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chip8_init(Chip8 *chip) { memset(chip, 0, sizeof(Chip8)); }
