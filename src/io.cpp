#include "io.hpp"
#include "chip8.hpp"

#include <SDL.h>
#include <cstdio>

#define PIXEL_ON_COLOR 0x00CE00FF
#define PIXEL_OFF_COLOR 0x000000FF


// SDL2 boiler plate for rendering
Display::Display(const char* name)
{
    SDL_Init(SDL_INIT_VIDEO);
    this->screen = SDL_CreateWindow(
        name, 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        0
    );
    this->renderer = SDL_CreateRenderer(screen, -1, 0);
    this->texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT
    );
}

// Destructor
Display::~Display(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(screen);
    SDL_Quit();
}

// Buffer frame before drawing
void Display::bufferDraw(Chip8* system)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            uint8_t px = system->display[y][x];
            system->buffer[(y * SCREEN_WIDTH) + x] = px == 0 ? PIXEL_OFF_COLOR : PIXEL_ON_COLOR;
        }
    }
}

// Draw by leveraging buffer
void Display::draw(Chip8* system)
{
    SDL_UpdateTexture(this->texture, NULL, system->buffer, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(this->renderer);
    SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
    SDL_RenderPresent(this->renderer);
    system->drawFlag = 0;
}

bool Display::processInput(Chip8* system)
{
    bool run = true;
    if (system->stepFlag) {
        while (1) {
            if (SDL_PollEvent(&this->e)) {
                if (this->e.type == SDL_QUIT) return 0;
                if (this->e.type == SDL_KEYDOWN) {
                    if (this->e.key.keysym.sym == SDLK_n) {
                        system->stepFlag = 0;
                        return 1;
                    }
                    return 1;
                }
            }
        }
    }

    if (SDL_PollEvent(&this->e)) {
        switch (this->e.type) {
            // Poll for quit
            case SDL_QUIT:
                run = false;
                break;

            // Poll for window 'X' interaction
            case SDL_WINDOWEVENT:
                switch(this->e.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        exit(0);

                    default:
                        break;
                }

            // Poll keydown
            case SDL_KEYDOWN:
                switch (this->e.key.keysym.sym) {
                    case SDLK_x:
                        if (system->debugFlag) { printf("[KEYDOWN] x\n"); }
                        system->keypad[0] = 1;
                        run = false;
                        break;
                    
                    case SDLK_1:
                        if (system->debugFlag) { printf("[KEYDOWN] 1\n"); }
                        system->keypad[1] = 1;
                        break;

                    case SDLK_2:
                        if (system->debugFlag) { printf("[KEYDOWN] 2\n"); }
                        system->keypad[2] = 1;
                        break;

                    case SDLK_3:
                        if (system->debugFlag) { printf("[KEYDOWN] 3\n"); }
                        system->keypad[3] = 1;
                        break;

                    case SDLK_q:
                        if (system->debugFlag) { printf("[KEYDOWN] q\n"); }
                        system->keypad[4] = 1;
                        break;

                    case SDLK_w:
                        if (system->debugFlag) { printf("[KEYDOWN] w\n"); }
                        system->keypad[5] = 1;
                        break;

                    case SDLK_e:
                        if (system->debugFlag) { printf("[KEYDOWN] e\n"); }
                        system->keypad[6] = 1;
                        break;

                    case SDLK_a:
                        if (system->debugFlag) { printf("[KEYDOWN] a\n"); }
                        system->keypad[7] = 1;
                        break;

                    case SDLK_s:
                        if (system->debugFlag) { printf("[KEYDOWN] s\n"); }
                        system->keypad[8] = 1;
                        break;

                    case SDLK_d:
                        if (system->debugFlag) { printf("[KEYDOWN] d\n"); }
                        system->keypad[9] = 1;
                        break;

                    case SDLK_z:
                        if (system->debugFlag) { printf("[KEYDOWN] z\n"); }
                        system->keypad[10] = 1;
                        break;

                    case SDLK_c:
                        if (system->debugFlag) { printf("[KEYDOWN] c\n"); }
                        system->keypad[11] = 1;
                        break;

                    case SDLK_4:
                        if (system->debugFlag) { printf("[KEYDOWN] 4\n"); }
                        system->keypad[12] = 1;
                        break;

                    case SDLK_r:
                        if (system->debugFlag) { printf("[KEYDOWN] 5\n"); }
                        system->keypad[13] = 1;
                        break;

                    case SDLK_f:
                        if (system->debugFlag) { printf("[KEYDOWN] 6\n"); }
                        system->keypad[14] = 1;
                        break;

                    case SDLK_v:
                        if (system->debugFlag) { printf("[KEYDOWN] 7\n"); }
                        system->keypad[15] = 1;
                        break;

                    case SDLK_EQUALS:
                        if (system->delayTimer > 0) system->delayTimer -= 1;
                        if (system->debugFlag) { printf("[KEYDOWN] Cycle delay: %d\n", system->delayTimer); }
                        break;

                    case SDLK_MINUS:
                        system->delayTimer += 1;
                        if (system->debugFlag) { printf("[KEYDOWN] Cycle delay: %d\n", system->delayTimer); }
                        break;
                }
                break;
            
            // Poll keyup
            case SDL_KEYUP:
                switch (this->e.key.keysym.sym) {
                    case SDLK_x:
                        if (system->debugFlag) { printf("[KEYUP] x\n"); }
                        system->keypad[0] = 0;
                        run = false;
                        break;
                    
                    case SDLK_1:
                        if (system->debugFlag) { printf("[KEYUP] 1\n"); }
                        system->keypad[1] = 0;
                        break;

                    case SDLK_2:
                        if (system->debugFlag) { printf("[KEYUP] 2\n"); }
                        system->keypad[2] = 0;
                        break;

                    case SDLK_3:
                        if (system->debugFlag) { printf("[KEYUP] 3\n"); }
                        system->keypad[3] = 0;
                        break;

                    case SDLK_q:
                        if (system->debugFlag) { printf("[KEYUP] 4\n"); }
                        system->keypad[4] = 0;
                        break;

                    case SDLK_w:
                        if (system->debugFlag) { printf("[KEYUP] 5\n"); }
                        system->keypad[5] = 0;
                        break;

                    case SDLK_e:
                        if (system->debugFlag) { printf("[KEYUP] e\n"); }
                        system->keypad[6] = 0;
                        break;

                    case SDLK_a:
                        if (system->debugFlag) { printf("[KEYUP] a\n"); }
                        system->keypad[7] = 0;
                        break;

                    case SDLK_s:
                        if (system->debugFlag) { printf("[KEYUP] s\n"); }
                        system->keypad[8] = 0;
                        break;

                    case SDLK_d:
                        if (system->debugFlag) { printf("[KEYUP] d\n"); }
                        system->keypad[9] = 0;
                        break;

                    case SDLK_z:
                        if (system->debugFlag) { printf("[KEYUP] z\n"); }
                        system->keypad[10] = 0;
                        break;

                    case SDLK_c:
                        if (system->debugFlag) { printf("[KEYUP] c\n"); }
                        system->keypad[11] = 0;
                        break;

                    case SDLK_4:
                        if (system->debugFlag) { printf("[KEYUP] 4\n"); }
                        system->keypad[12] = 0;
                        break;

                    case SDLK_r:
                        if (system->debugFlag) { printf("[KEYUP] r\n"); }
                        system->keypad[13] = 0;
                        break;

                    case SDLK_f:
                        if (system->debugFlag) { printf("[KEYUP] f\n"); }
                        system->keypad[14] = 0;
                        break;

                    case SDLK_v:
                        if (system->debugFlag) { printf("[KEYUP] v\n"); }
                        system->keypad[15] = 0;
                        break;

                    case SDLK_m:
                        system->stepFlag = 1;
                        break;
                }
                break;
        }
    }
    return run;
}

void Display::haltAndAwaitKey(Chip8 *system)
{
    while(true) {
        this->processInput(system);
        for (int i = 0; i < 16; i++) {
            if (system->keypad[i]) {
                goto end_loop;
            }
        }
    }
    
    end_loop:
    return;
}
