#include "SDL_events.h"
#include "SDL_timer.h"
#include "chip8.h"
#include <SDL.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>

#define SCALE 10
#define SCREEN_WIDTH (DISPLAY_WIDTH * SCALE)
#define SCREEN_HEIGHT (DISPLAY_HEIGHT * SCALE)

Chip8 chip;

void renderer_init(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void render_display(void *userdata) {
  SDL_Renderer *renderer = (SDL_Renderer *)userdata;
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
void handle_key_events(SDL_Event e, uint8_t *keypad) {
  bool pressed = e.type == SDL_KEYDOWN;
  switch (e.key.keysym.scancode) {
  case SDL_SCANCODE_1:
    keypad[0x1] = pressed;
    break;
  case SDL_SCANCODE_2:
    keypad[0x2] = pressed;
    break;
  case SDL_SCANCODE_3:
    keypad[0x3] = pressed;
    break;
  case SDL_SCANCODE_4:
    keypad[0xC] = pressed;
    break;

  case SDL_SCANCODE_Q:
    keypad[0x4] = pressed;
    break;
  case SDL_SCANCODE_W:
    keypad[0x5] = pressed;
    break;
  case SDL_SCANCODE_E:
    keypad[0x6] = pressed;
    break;
  case SDL_SCANCODE_R:
    keypad[0xD] = pressed;
    break;

  case SDL_SCANCODE_A:
    keypad[0x7] = pressed;
    break;
  case SDL_SCANCODE_S:
    keypad[0x8] = pressed;
    break;
  case SDL_SCANCODE_D:
    keypad[0x9] = pressed;
    break;
  case SDL_SCANCODE_F:
    keypad[0xE] = pressed;
    break;

  case SDL_SCANCODE_Z:
    keypad[0xA] = pressed;
    break;
  case SDL_SCANCODE_X:
    keypad[0x0] = pressed;
    break;
  case SDL_SCANCODE_C:
    keypad[0xB] = pressed;
    break;
  case SDL_SCANCODE_V:
    keypad[0xF] = pressed;
    break;

  default:; // noop
  }
}

bool handle_sdl_events(uint8_t *keypad) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return false;
    }
    if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN) {
      handle_key_events(e, keypad);
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

  if (argc < 2) {
    printf("Usage: %s <ROM>\n", argv[0]);
    return 1;
  }

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

  chip8_init(&chip);

  int load_err = chip8_load_rom(&chip, argv[1]);
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
