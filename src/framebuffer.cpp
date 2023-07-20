#include <vector>
#include "framebuffer.h"

const int FRAMEBUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

// Puntos de prueba
int x = 10;
int y = 10;

Color framebuffer[FRAMEBUFFER_SIZE];
Color clearColor(0, 0, 0);
Color currentColor(255, 255, 255);

void clear() {
    for (auto & i : framebuffer) {
        i = clearColor;
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
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        framebuffer[y * SCREEN_WIDTH + x] = currentColor;
    }
}

void renderBuffer(SDL_Renderer* renderer) {
    // Create a texture
    SDL_Texture* texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH,
            SCREEN_HEIGHT
    );

    // Update the texture with the pixel data from the framebuffer
    SDL_UpdateTexture(
            texture,
            NULL,
            framebuffer,
            SCREEN_WIDTH * sizeof(Color)
    );

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}

void render(SDL_Renderer* renderer) {
    // Clear the framebuffer
    clear();

    // Render the framebuffer to the screen
    renderBuffer(renderer);
}