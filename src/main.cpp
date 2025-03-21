#include "chip8.hpp"
#include "io.hpp"

#include <SDL.h>
#include <cstdio>

#ifdef IS_WINDOWS
#include <windows.h>
#elif IS_LINUX
#include <unistd.h>
#endif

int main(int argc, char** argv)
{
    // Check argc to ensure arg was provided for rom filepath
    if (argc != 3) {
        printf("Usage: chip8 [int: debug_flag] [string: rom.ch8]\n");
        return 1;
    }

    bool debugFlag = false;
    if (atoi(argv[1])) debugFlag = true;

    // Init display for IO
    Display io{"Chipper"};

    // Init chip8 system struct and initialize
    Chip8 chip8{debugFlag, 20, &io};

    // Load roam into chip8 system
    chip8.loadRom(argv[2]);

    // Emulation loop
    bool run = true;
    while (run) {
        // Event polling from SDL
        run = io.processInput(&chip8);
        
        // Fetch, decode, execute cycle
        chip8.emulateCycle();
        
        // Draw if flag is set
        if (chip8.drawFlag) {
            io.bufferDraw(&chip8);
            io.draw(&chip8);
        }

        // Timing
#ifdef IS_WINDOWS
        Sleep(chip8.delayTimer);
#elif IS_LINUX
        usleep(chip8.delay_timer);
#endif
    }

    return 0;
}