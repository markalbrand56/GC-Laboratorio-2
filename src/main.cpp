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

// Define the initial pattern (Glider)
void setInitialPattern() {
    // Glider pattern
    setCellAlive(1, 0);
    setCellAlive(2, 1);
    setCellAlive(0, 2);
    setCellAlive(1, 2);
    setCellAlive(2, 2);
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
