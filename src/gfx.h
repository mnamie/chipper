#ifndef GFX_H_
#define GFX_H_

#include <SDL2/SDL.h>

#include "chip8.h"

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 1280

void init_display();
void buffer_draw(Chip8* system);
void draw(Chip8* system);
uint process_input(Chip8* system);

#endif
