#include <SDL.h>

// Define the size of the framebuffer
const int FRAMEBUFFER_WIDTH = 100;
const int FRAMEBUFFER_HEIGHT = 100;
const int FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Define a Color struct to hold the RGB values of a pixel
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Declare the framebuffer as a global variable
Color framebuffer[FRAMEBUFFER_SIZE];

// Declare a global clearColor of type Color
Color clearColor = {0, 0, 0, 255}; // Initially set to black

// Declare a global currentColor of type Color
Color currentColor = {255, 255, 255, 255}; // Initially set to white

// Function to clear the framebuffer with the clearColor
void clear() {
    for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
        framebuffer[i] = clearColor;
    }
}

// Function to set a specific pixel in the framebuffer to the currentColor
void point(int x, int y) {
    if (x >= 0 && x < FRAMEBUFFER_WIDTH && y >= 0 && y < FRAMEBUFFER_HEIGHT) {
        framebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
    }
}

// Define the grid size for the Game of Life
const int GRID_WIDTH = 100;
const int GRID_HEIGHT = 100;

// Declare the grid as a global variable
bool grid[GRID_HEIGHT][GRID_WIDTH] = {false}; // Initially all cells are dead

// Function to set a specific cell in the grid as alive
void setCellAlive(int x, int y) {
    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
        grid[y][x] = true;
    }
}

// Function to apply the rules of Conway's Game of Life to update the grid
void applyRules() {
    bool newGrid[GRID_HEIGHT][GRID_WIDTH] = {false};

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int aliveNeighbors = 0;

            // Check the number of alive neighbors for each cell
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (!(i == 0 && j == 0)) {
                        int neighborX = (x + i + GRID_WIDTH) % GRID_WIDTH;
                        int neighborY = (y + j + GRID_HEIGHT) % GRID_HEIGHT;

                        if (grid[neighborY][neighborX]) {
                            aliveNeighbors++;
                        }
                    }
                }
            }

            // Apply the rules to update the new grid
            if (grid[y][x]) {
                if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                    newGrid[y][x] = false; // cell dies due to underpopulation or overpopulation
                } else {
                    newGrid[y][x] = true; // cell survives
                }
            } else {
                if (aliveNeighbors == 3) {
                    newGrid[y][x] = true; // cell reproduces
                }
            }
        }
    }

    // Update the grid with the new state
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = newGrid[y][x];
        }
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
    // Uncomment this line if you want to clear the framebuffer in each frame
    // clear();

    // Apply the rules of Conway's Game of Life to update the grid
    applyRules();

    // Draw the cells in the grid to the framebuffer
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x]) {
                // Set the currentColor to white for alive cells
                currentColor = {255, 255, 255, 255};
            } else {
                // Set the currentColor to black for dead cells
                currentColor = {0, 0, 0, 255};
            }

            // Draw the cell using the point function
            point(x, y);
        }
    }

    // Render the framebuffer to the screen
    renderBuffer(renderer);
}

// Patterns

void placeGlider(int startX, int startY) {
    setCellAlive(startX + 1, startY);
    setCellAlive(startX + 2, startY + 1);
    setCellAlive(startX, startY + 2);
    setCellAlive(startX + 1, startY + 2);
    setCellAlive(startX + 2, startY + 2);
}

void placeBlock(int startX, int startY) {
    setCellAlive(startX, startY);
    setCellAlive(startX + 1, startY);
    setCellAlive(startX, startY + 1);
    setCellAlive(startX + 1, startY + 1);
}

void placeBeehive(int startX, int startY) {
    setCellAlive(startX + 1, startY);
    setCellAlive(startX + 2, startY);
    setCellAlive(startX, startY + 1);
    setCellAlive(startX + 3, startY + 1);
    setCellAlive(startX + 1, startY + 2);
    setCellAlive(startX + 2, startY + 2);
}

void placeLoaf(int startX, int startY) {
    setCellAlive(startX + 1, startY);
    setCellAlive(startX + 2, startY);
    setCellAlive(startX, startY + 1);
    setCellAlive(startX + 3, startY + 1);
    setCellAlive(startX + 1, startY + 2);
    setCellAlive(startX + 3, startY + 2);
    setCellAlive(startX + 2, startY + 3);
}

void placeBlinker(int startX, int startY) {
    setCellAlive(startX, startY);
    setCellAlive(startX + 1, startY);
    setCellAlive(startX + 2, startY);
}

// Function to place a toad at a specific position in the grid
void placeToad(int startX, int startY) {
    setCellAlive(startX + 1, startY);
    setCellAlive(startX + 2, startY);
    setCellAlive(startX + 3, startY);
    setCellAlive(startX, startY + 1);
    setCellAlive(startX + 1, startY + 1);
    setCellAlive(startX + 2, startY + 1);
}

// Function to place a beacon at a specific position in the grid
void placeBeacon(int startX, int startY) {
    setCellAlive(startX, startY);
    setCellAlive(startX + 1, startY);
    setCellAlive(startX, startY + 1);
    setCellAlive(startX + 3, startY + 2);
    setCellAlive(startX + 2, startY + 3);
    setCellAlive(startX + 3, startY + 3);
}

// Function to place a pulsar at a specific position in the grid
void placePulsar(int startX, int startY) {
    // Upper-left quadrant
    setCellAlive(startX + 2, startY);
    setCellAlive(startX + 3, startY);
    setCellAlive(startX + 4, startY);
    setCellAlive(startX, startY + 2);
    setCellAlive(startX + 5, startY + 2);
    setCellAlive(startX, startY + 3);
    setCellAlive(startX + 5, startY + 3);
    setCellAlive(startX, startY + 4);
    setCellAlive(startX + 5, startY + 4);
    setCellAlive(startX + 2, startY + 5);
    setCellAlive(startX + 3, startY + 5);
    setCellAlive(startX + 4, startY + 5);

    // Upper-right quadrant
    setCellAlive(startX + 9, startY);
    setCellAlive(startX + 10, startY);
    setCellAlive(startX + 11, startY);
    setCellAlive(startX + 7, startY + 2);
    setCellAlive(startX + 12, startY + 2);
    setCellAlive(startX + 7, startY + 3);
    setCellAlive(startX + 12, startY + 3);
    setCellAlive(startX + 7, startY + 4);
    setCellAlive(startX + 12, startY + 4);
    setCellAlive(startX + 9, startY + 5);
    setCellAlive(startX + 10, startY + 5);
    setCellAlive(startX + 11, startY + 5);

    // Lower-left quadrant
    setCellAlive(startX + 2, startY + 7);
    setCellAlive(startX + 3, startY + 7);
    setCellAlive(startX + 4, startY + 7);
    setCellAlive(startX, startY + 9);
    setCellAlive(startX + 5, startY + 9);
    setCellAlive(startX, startY + 10);
    setCellAlive(startX + 5, startY + 10);
    setCellAlive(startX, startY + 11);
    setCellAlive(startX + 5, startY + 11);
    setCellAlive(startX + 2, startY + 12);
    setCellAlive(startX + 3, startY + 12);
    setCellAlive(startX + 4, startY + 12);

    // Lower-right quadrant
    setCellAlive(startX + 9, startY + 7);
    setCellAlive(startX + 10, startY + 7);
    setCellAlive(startX + 11, startY + 7);
    setCellAlive(startX + 7, startY + 9);
    setCellAlive(startX + 12, startY + 9);
    setCellAlive(startX + 7, startY + 10);
    setCellAlive(startX + 12, startY + 10);
    setCellAlive(startX + 7, startY + 11);
    setCellAlive(startX + 12, startY + 11);
    setCellAlive(startX + 9, startY + 12);
    setCellAlive(startX + 10, startY + 12);
    setCellAlive(startX + 11, startY + 12);
}

// Function to place a penthadecathlon at a specific position in the grid
void placePentadecathlon(int startX, int startY) {
    setCellAlive(startX + 1, startY);
    setCellAlive(startX + 2, startY);
    setCellAlive(startX + 4, startY);
    setCellAlive(startX + 5, startY);
    setCellAlive(startX + 6, startY);
    setCellAlive(startX + 7, startY);
    setCellAlive(startX + 9, startY);
    setCellAlive(startX + 10, startY);
    setCellAlive(startX + 11, startY);
    setCellAlive(startX + 12, startY);
    setCellAlive(startX + 13, startY);
    setCellAlive(startX + 14, startY);
    setCellAlive(startX + 15, startY);
    setCellAlive(startX + 16, startY);
    setCellAlive(startX + 17, startY);
}

// Function to place a pulsar2 at a specific position in the grid
void placePulsar2(int startX, int startY) {
    setCellAlive(startX + 2, startY);
    setCellAlive(startX + 3, startY);
    setCellAlive(startX + 4, startY);
    setCellAlive(startX + 8, startY);
    setCellAlive(startX + 9, startY);
    setCellAlive(startX + 10, startY);

    setCellAlive(startX, startY + 2);
    setCellAlive(startX + 5, startY + 2);
    setCellAlive(startX + 7, startY + 2);
    setCellAlive(startX + 12, startY + 2);

    setCellAlive(startX, startY + 3);
    setCellAlive(startX + 5, startY + 3);
    setCellAlive(startX + 7, startY + 3);
    setCellAlive(startX + 12, startY + 3);

    setCellAlive(startX + 2, startY + 4);
    setCellAlive(startX + 3, startY + 4);
    setCellAlive(startX + 4, startY + 4);
    setCellAlive(startX + 8, startY + 4);
    setCellAlive(startX + 9, startY + 4);
    setCellAlive(startX + 10, startY + 4);

    setCellAlive(startX + 2, startY + 5);
    setCellAlive(startX + 3, startY + 5);
    setCellAlive(startX + 4, startY + 5);
    setCellAlive(startX + 8, startY + 5);
    setCellAlive(startX + 9, startY + 5);
    setCellAlive(startX + 10, startY + 5);

    setCellAlive(startX, startY + 7);
    setCellAlive(startX + 5, startY + 7);
    setCellAlive(startX + 7, startY + 7);
    setCellAlive(startX + 12, startY + 7);

    setCellAlive(startX, startY + 8);
    setCellAlive(startX + 5, startY + 8);
    setCellAlive(startX + 7, startY + 8);
    setCellAlive(startX + 12, startY + 8);

    setCellAlive(startX + 2, startY + 9);
    setCellAlive(startX + 3, startY + 9);
    setCellAlive(startX + 4, startY + 9);
    setCellAlive(startX + 8, startY + 9);
    setCellAlive(startX + 9, startY + 9);
    setCellAlive(startX + 10, startY + 9);

    setCellAlive(startX + 2, startY + 11);
    setCellAlive(startX + 3, startY + 11);
    setCellAlive(startX + 4, startY + 11);
    setCellAlive(startX + 8, startY + 11);
    setCellAlive(startX + 9, startY + 11);
    setCellAlive(startX + 10, startY + 11);
}

// Function to place a lightweight spaceship at a specific position in the grid
void placeLWSS(int startX, int startY) {
    setCellAlive(startX + 1, startY);
    setCellAlive(startX + 4, startY);

    setCellAlive(startX, startY + 1);
    setCellAlive(startX + 4, startY + 1);

    setCellAlive(startX + 4, startY + 2);

    setCellAlive(startX, startY + 3);
    setCellAlive(startX + 1, startY + 3);
    setCellAlive(startX + 2, startY + 3);
    setCellAlive(startX + 3, startY + 3);
}

// Function to place a heavyweight spaceship at a specific position in the grid
void placeHWSS(int startX, int startY) {
    setCellAlive(startX + 1, startY);
    setCellAlive(startX + 5, startY);

    setCellAlive(startX + 6, startY + 1);
    setCellAlive(startX + 6, startY + 2);
    setCellAlive(startX, startY + 3);
    setCellAlive(startX + 6, startY + 3);
    setCellAlive(startX + 3, startY + 4);
    setCellAlive(startX + 4, startY + 4);
    setCellAlive(startX + 5, startY + 4);
    setCellAlive(startX + 6, startY + 4);
}


// Define the initial pattern (Glider)
void setInitialPattern() {

}

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Game of Life", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
    );

    bool running = true;
    SDL_Event event;

    // Set the initial pattern (Glider) on the grid
    setInitialPattern();

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
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
