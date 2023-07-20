#include <iostream>
#include <SDL.h>
#include "framebuffer.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

void defineInitialPattern() {
    // Clear the framebuffer first
    clear();

    // Define the glider pattern
    point({10, 10});
    point({11, 11});
    point({9, 12});
    point({10, 12});
    point({11, 12});
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("life", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
    );

    bool running = true;
    SDL_Event event;

    // Define the initial pattern
    defineInitialPattern();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Call our render function
        render(renderer);

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}