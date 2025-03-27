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

void sdl_input(uint8_t *keypad) {
  // TODO
}

bool handle_sdl_events() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN) {
      return false;
    }
  }
  return true;
}

void handle_sigint(int sig) {
  printf("\nInterrupt signal detected. Exiting...\n");
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
  // TEST set first pixel to white
  chip.display[0] = 1;

  int load_err = chip8_load_rom(&chip, argv[1]);
  if (load_err) {
    return load_err;
  }

  chip8_run(&chip, render_display, sdl_input, handle_sdl_events, renderer);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
