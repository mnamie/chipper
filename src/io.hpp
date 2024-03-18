#ifndef _GFX_HPP_
#define _GFX_HPP_

#include <SDL.h>

#include "chip8.hpp"

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 1280

class Chip8;
class Display
{
public:
    SDL_Window* screen;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Event e;

    Display(const char *);
    void bufferDraw(Chip8*);
    void draw(Chip8*);
    bool processInput(Chip8*);
    void haltAndAwaitKey(Chip8*);
    void destroy();
};

#endif