#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Hello, SDL2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load a font (Make sure you have a TTF font file, e.g., "Arial.ttf")
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 48);
    if (!font) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    // Create text surface & texture
    SDL_Color textColor = {255, 255, 255, 255}; // White color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hello, SDL2!", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Get text dimensions
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface); // No longer needed

    // Position the text in the center
    SDL_Rect textRect = { (SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight };

    // Main loop
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN) {
                quit = 1;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Render text
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

