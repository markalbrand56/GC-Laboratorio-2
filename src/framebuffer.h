#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include "color.h"

// Tamaño de la pantalla
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

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