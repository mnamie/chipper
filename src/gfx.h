#ifndef GFX_H_
#define GFX_H_

#include <SDL2/SDL.h>

#include "chip8.h"

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 1280

// Required globals for rendering with SDL2
// typedef struct Chip8IO {
//     SDL_Window* screen;
//     SDL_Renderer* renderer;
//     SDL_Texture* texture;
// } Chip8IO;

void init_display();
void buffer_draw(Chip8* system);
void draw(Chip8* system);
uint process_input(Chip8* system);

#endif
