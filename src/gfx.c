#include "gfx.h"
#include "chip8.h"

#include <SDL2/SDL.h>

#define MULT 8

// Required globals for rendering with SDL2
SDL_Window* screen;
SDL_Renderer* renderer;
SDL_Texture* texture;

SDL_Scancode keymappings[16] = {
    SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
    SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
    SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V};

// SDL2 boiler plate for rendering
void init_display()
{
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_CreateWindow("Chip-8 Emu", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        0);
    renderer = SDL_CreateRenderer(screen, -1, 0);
    texture = SDL_CreateTexture(renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT
    );
}

// Buffer frame before drawing
void buffer_draw(Chip8* system)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            uint8_t px = system->display[y][x];
            system->buffer[(y * SCREEN_WIDTH) + x] = (0xFFFFFF00 * px) | 0x000000FF;
        }
    }
}

// Draw by leveraging buffer
void draw(Chip8* system) 
{
    SDL_UpdateTexture(texture, NULL, system->buffer, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}