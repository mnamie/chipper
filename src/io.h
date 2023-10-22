#ifndef _GFX_H_
#define _GFX_H_

#include <SDL2/SDL.h>

#include "chip8.h"

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 1280

void init_display(const char *name);
void buffer_draw(Chip8 *system);
void draw(Chip8 *system);
uint process_input(Chip8 *system, SDL_Event *e);
void halt_and_await_key(Chip8 *system);

#endif
