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

void render(SDL_Renderer* renderer) {
    // Create a copy of the current framebuffer to calculate the next state
    std::vector<Color> currentFramebuffer(framebuffer, framebuffer + FRAMEBUFFER_SIZE);

    // Clear the framebuffer
    clear();

    // Apply Conway's Game of Life rules to update the next state of the cells
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; ++y) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; ++x) {
            int liveNeighbors = 0;

            // Count the number of live neighbors around the current cell
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0) continue; // Skip the current cell
                    int neighborX = x + i;
                    int neighborY = y + j;

                    // Check if the neighbor is within the bounds of the framebuffer
                    if (neighborX >= 0 && neighborX < FRAMEBUFFER_WIDTH && neighborY >= 0 && neighborY < FRAMEBUFFER_HEIGHT) {
                        if (currentFramebuffer[neighborY * FRAMEBUFFER_WIDTH + neighborX].r == 255) {
                            liveNeighbors++;
                        }
                    }
                }
            }

            // Apply the Game of Life rules
            if (currentFramebuffer[y * FRAMEBUFFER_WIDTH + x].r == 255) { // Current cell is alive
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    // Underpopulation or Overpopulation, the cell dies
                    point({x, y}); // Set the current cell to dead (black)
                } else {
                    // Survival, the cell remains alive
                    point({x, y}); // Set the current cell to alive (white)
                }
            } else { // Current cell is dead
                if (liveNeighbors == 3) {
                    // Reproduction, a new cell is born
                    point({x, y}); // Set the current cell to alive (white)
                }
            }
        }
    }

    // Render the framebuffer to the screen
    renderBuffer(renderer);
}