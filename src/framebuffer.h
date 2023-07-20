#pragma once
#include <string>
#include <vector>
#include <SDL.h>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Tamaño de la pantalla
const int FRAMEBUFFER_WIDTH = 100;
const int FRAMEBUFFER_HEIGHT = 100;

// Definiciones de color y estructuras
struct Vertex2 {
    int x, y;
};

// Variables globales del framebuffer
extern Color clearColor;
extern Color currentColor;

// Funciones del framebuffer
void clear();
void setCurrentColor(Color color);
void setClearColor(Color color);
void point(Vertex2 vertex);

// Render

void renderBuffer(SDL_Renderer* renderer);
void render(SDL_Renderer* renderer);