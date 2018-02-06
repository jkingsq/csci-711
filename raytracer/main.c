#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

SDL_Window* window = NULL;

SDL_Surface* windowSurface = NULL;

int startSDL() {
    int success = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to start SDL2:\n%s\n\n", SDL_GetError());
        success = 1;
    } else {
        window = SDL_CreateWindow("Global Illumination",
        SDL_WINDOWPOS_UNDEFINED, // window x
        SDL_WINDOWPOS_UNDEFINED, // window y
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN); // flags

        if(window == NULL) {
            fprintf(stderr, "Failed to create window:\n%s\n\n", SDL_GetError());
            success = 1;
        } else {
            windowSurface = SDL_GetWindowSurface(window);
        }
    }
    return success;
}

int loadImages() {
}

void quit() {
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if(startSDL()) {
        fprintf(stderr, "Failed to initialize.  Exiting.\n");
    } else {
        SDL_UpdateWindowSurface(window);
        SDL_Delay(2000);
    }
    quit();
    return 0;
}
