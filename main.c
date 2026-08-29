#include "SDL_events.h"
#include "SDL_timer.h"
#include <SDL.h>

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>

#include "chip8.h"

#define SCALE 10
#define SCREEN_WIDTH (DISPLAY_WIDTH * SCALE)
#define SCREEN_HEIGHT (DISPLAY_HEIGHT * SCALE)

/**
 * the emulator instance
 */
Chip8 chip;

/**
 * original chip8 quirks flags
 */
const Chip8Quirks vip_quirks = {
    .shift_uses_vx = false,
    .load_store_increment_i = true,
    .logic_resets_vf = true,
    .clip_sprites = true,
    .draw_waits_for_vblank = true,
};

/**
 * octo quirks flags
 */
const Chip8Quirks octo_quirks = {
    .shift_uses_vx = false,
    .load_store_increment_i = true,
    .logic_resets_vf = false,
    .clip_sprites = false,
    .draw_waits_for_vblank = false,
};

void renderer_init(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void render_display(void *userdata) {
  SDL_Renderer *renderer = (SDL_Renderer *)userdata;

  // clear screen before redrawing
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  for (int y = 0; y < DISPLAY_HEIGHT; y++) {
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
      if (chip.display[y * DISPLAY_WIDTH + x]) {
        SDL_Rect rect = {x * SCALE, y * SCALE, SCALE, SCALE};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

/**
 * map top left of keyboard to original COSMAC VIP hex keyboard:
 *
 * 1 2 3 4  ->  1 2 3 C
 * Q W E R  ->  4 5 6 D
 * A S D F  ->  7 8 9 E
 * Z X C V  ->  A 0 B F
 *
 * scancodes are used instead of keycodes so that the mapping
 * will work the same under QWERTY, AZERTY, or other layouts
 */
void handle_key_events(SDL_Event e, Chip8 *chip) {
  Chip8EventType dir = e.type == SDL_KEYDOWN ? CHIP8_KEY_DOWN : CHIP8_KEY_UP;
  switch (e.key.keysym.scancode) {
  case SDL_SCANCODE_1:
    chip8_key_event(chip, 0x1, dir);
    break;
  case SDL_SCANCODE_2:
    chip8_key_event(chip, 0x2, dir);
    break;
  case SDL_SCANCODE_3:
    chip8_key_event(chip, 0x3, dir);
    break;
  case SDL_SCANCODE_4:
    chip8_key_event(chip, 0xC, dir);
    break;

  case SDL_SCANCODE_Q:
    chip8_key_event(chip, 0x4, dir);
    break;
  case SDL_SCANCODE_W:
    chip8_key_event(chip, 0x5, dir);
    break;
  case SDL_SCANCODE_E:
    chip8_key_event(chip, 0x6, dir);
    break;
  case SDL_SCANCODE_R:
    chip8_key_event(chip, 0xD, dir);
    break;

  case SDL_SCANCODE_A:
    chip8_key_event(chip, 0x7, dir);
    break;
  case SDL_SCANCODE_S:
    chip8_key_event(chip, 0x8, dir);
    break;
  case SDL_SCANCODE_D:
    chip8_key_event(chip, 0x9, dir);
    break;
  case SDL_SCANCODE_F:
    chip8_key_event(chip, 0xE, dir);
    break;

  case SDL_SCANCODE_Z:
    chip8_key_event(chip, 0xA, dir);
    break;
  case SDL_SCANCODE_X:
    chip8_key_event(chip, 0x0, dir);
    break;
  case SDL_SCANCODE_C:
    chip8_key_event(chip, 0xB, dir);
    break;
  case SDL_SCANCODE_V:
    chip8_key_event(chip, 0xF, dir);
    break;

  default:; // noop
  }
}

bool handle_sdl_events(Chip8 *chip) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return false;
    }
    if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN) {
      handle_key_events(e, chip);
    }
  }
  return true;
}

void handle_sigint(int sig) {
  printf("\nInterrupt signal %i detected. Exiting...\n", sig);
  exit(0);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, handle_sigint);

  const char *rom_path = NULL;
  bool debug = false;
  double clock_speed = 6000.0;
  const Chip8Quirks *profile = &octo_quirks;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--debug") == 0) {
      debug = true;

    } else if (strcmp(argv[i], "--clock-speed") == 0) {
      if (i + 1 >= argc) {
        fprintf(stderr, "Missing value for --clock-speed\n");
        return 1;
      }

      clock_speed = atof(argv[++i]);

    } else if (strcmp(argv[i], "--profile") == 0) {
      if (i + 1 >= argc) {
        fprintf(stderr, "Missing value for --profile\n");
        return 1;
      }

      if (strcmp(argv[i + 1], "vip") == 0) {
        profile = &vip_quirks;
      } else if (strcmp(argv[i + 1], "octo") == 0) {
        // no op, octo is default
        printf("got here\n");
      } else {
        fprintf(stderr, "Unknown profile: %s\n", argv[i]);
      }
      i++;

    } else {
      if (rom_path == NULL) {
        rom_path = argv[i];
      } else {
        fprintf(stderr, "Unknown extra argument: %s\n", argv[i]);
        return 1;
      }
    }
  }

  if (!rom_path) {
    fprintf(stderr, "Usage: %s <rom_file> [--debug] [--clock-speed Hz]\n",
            argv[0]);
    return 1;
  }

  printf("ROM: %s\n", rom_path);
  printf("Debug mode: %s\n", debug ? "ON" : "OFF");
  printf("Clock speed: %.1f Hz\n", clock_speed);
  printf("Profile: %s\n", profile == &vip_quirks ? "vip" : "octo");

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!window) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!renderer) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  renderer_init(renderer);

  chip8_init(&chip, clock_speed, debug, profile);

  int load_err = chip8_load_rom(&chip, rom_path);
  if (load_err) {
    return load_err;
  }

  chip8_run(&chip, render_display, handle_sdl_events, SDL_GetTicks64, SDL_Delay,
            renderer);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
