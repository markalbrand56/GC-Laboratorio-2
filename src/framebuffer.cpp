#include <vector>
#include "framebuffer.h"

const int FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;

Color framebuffer[FRAMEBUFFER_SIZE];
Color clearColor = {0, 0, 0, 255}; // Initially set to black
Color currentColor = {255, 255, 255, 255}; // Initially set to white

void clear() {
    for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
        framebuffer[i] = clearColor;
    }
}

void setCurrentColor(Color color) {
    currentColor = color;
}

void setClearColor(Color color) {
    clearColor = color;
}

void point(Vertex2 vertex) {
    int x = vertex.x;
    int y = vertex.y;
    if (x >= 0 && x < FRAMEBUFFER_WIDTH && y >= 0 && y < FRAMEBUFFER_HEIGHT) {
        framebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
    }
}

void renderBuffer(SDL_Renderer* renderer) {
    // Create a texture
    SDL_Texture* texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STREAMING,
            FRAMEBUFFER_WIDTH,
            FRAMEBUFFER_HEIGHT
    );

    // Update the texture with the pixel data from the framebuffer
    SDL_UpdateTexture(
            texture,
            NULL,
            framebuffer,
            FRAMEBUFFER_WIDTH * sizeof(Color)
    );

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}

int x = 0;
int y = 0;

void render(SDL_Renderer* renderer) {
    // Create a copy of the current framebuffer to calculate the next state
    std::vector<Color> currentFramebuffer(framebuffer, framebuffer + FRAMEBUFFER_SIZE);

    // Clear the framebuffer
    clear();

    point({x++,y++});

    // Render the framebuffer to the screen
    renderBuffer(renderer);
}