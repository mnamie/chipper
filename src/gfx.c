#include "gfx.h"
#include "chip8.h"

#include <SDL2/SDL.h>

SDL_Window* screen;
SDL_Renderer* renderer;
SDL_Texture* texture;

// SDL2 boiler plate for rendering
void init_display()
{
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_CreateWindow(
        "Chipper", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN
    );
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(
        renderer, 
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

uint process_input(Chip8* system)
{
    SDL_Event e;
    uint run = 1;

    while(SDL_PollEvent(&e)) {
        switch (e.type) {
            // Poll for quit
            case SDL_QUIT:
                run = 0;
                break;

            // Poll keydown
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_x:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] x\n"); }
                        system->keypad[0] = 1;
                        break;
                    
                    case SDLK_1:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] 1\n"); }
                        system->keypad[1] = 1;
                        break;

                    case SDLK_2:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] 2\n"); }
                        system->keypad[2] = 1;
                        break;

                    case SDLK_3:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] 3\n"); }
                        system->keypad[3] = 1;
                        break;

                    case SDLK_q:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] q\n"); }
                        system->keypad[4] = 1;
                        break;

                    case SDLK_w:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] w\n"); }
                        system->keypad[5] = 1;
                        break;

                    case SDLK_e:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] e\n"); }
                        system->keypad[6] = 1;
                        break;

                    case SDLK_a:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] a\n"); }
                        system->keypad[7] = 1;
                        break;

                    case SDLK_s:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] s\n"); }
                        system->keypad[8] = 1;
                        break;

                    case SDLK_d:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] d\n"); }
                        system->keypad[9] = 1;
                        break;

                    case SDLK_z:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] z\n"); }
                        system->keypad[10] = 1;
                        break;

                    case SDLK_c:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] c\n"); }
                        system->keypad[11] = 1;
                        break;

                    case SDLK_4:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] 4\n"); }
                        system->keypad[12] = 1;
                        break;

                    case SDLK_r:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] 5\n"); }
                        system->keypad[13] = 1;
                        break;

                    case SDLK_f:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] 6\n"); }
                        system->keypad[14] = 1;
                        break;

                    case SDLK_v:
                        if (system->debug_flag == 1) { printf("[KEYDOWN] 7\n"); }
                        system->keypad[15] = 1;
                        break;
                }
                break;
            
            // Poll keyup
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_x:
                        if (system->debug_flag == 1) { printf("[KEYUP] x\n"); }
                        system->keypad[0] = 0;
                        break;
                    
                    case SDLK_1:
                        if (system->debug_flag == 1) { printf("[KEYUP] 1\n"); }
                        system->keypad[1] = 0;
                        break;

                    case SDLK_2:
                        if (system->debug_flag == 1) { printf("[KEYUP] 2\n"); }
                        system->keypad[2] = 0;
                        break;

                    case SDLK_3:
                        if (system->debug_flag == 1) { printf("[KEYUP] 3\n"); }
                        system->keypad[3] = 0;
                        break;

                    case SDLK_q:
                        if (system->debug_flag == 1) { printf("[KEYUP] 4\n"); }
                        system->keypad[4] = 0;
                        break;

                    case SDLK_w:
                        if (system->debug_flag == 1) { printf("[KEYUP] 5\n"); }
                        system->keypad[5] = 0;
                        break;

                    case SDLK_e:
                        if (system->debug_flag == 1) { printf("[KEYUP] e\n"); }
                        system->keypad[6] = 0;
                        break;

                    case SDLK_a:
                        if (system->debug_flag == 1) { printf("[KEYUP] a\n"); }
                        system->keypad[7] = 0;
                        break;

                    case SDLK_s:
                        if (system->debug_flag == 1) { printf("[KEYUP] s\n"); }
                        system->keypad[8] = 0;
                        break;

                    case SDLK_d:
                        if (system->debug_flag == 1) { printf("[KEYUP] d\n"); }
                        system->keypad[9] = 0;
                        break;

                    case SDLK_z:
                        if (system->debug_flag == 1) { printf("[KEYUP] z\n"); }
                        system->keypad[10] = 0;
                        break;

                    case SDLK_c:
                        if (system->debug_flag == 1) { printf("[KEYUP] c\n"); }
                        system->keypad[11] = 0;
                        break;

                    case SDLK_4:
                        if (system->debug_flag == 1) { printf("[KEYUP] 4\n"); }
                        system->keypad[12] = 0;
                        break;

                    case SDLK_r:
                        if (system->debug_flag == 1) { printf("[KEYUP] r\n"); }
                        system->keypad[13] = 0;
                        break;

                    case SDLK_f:
                        if (system->debug_flag == 1) { printf("[KEYUP] f\n"); }
                        system->keypad[14] = 0;
                        break;

                    case SDLK_v:
                        if (system->debug_flag == 1) { printf("[KEYUP] v\n"); }
                        system->keypad[15] = 0;
                        break;
                }
                break;
        }
    }
    return run;
}