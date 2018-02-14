#include <SDL2/SDL.h>
#include <stdio.h>

#include "Matrix.h"
#include "Solid.h"
#include "Vector.h"
#include "Ray.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
SDL_Color black = {0, 0, 0, 255};

SDL_Rect frame = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

SDL_Window *window = NULL;

SDL_Surface *windowSurface = NULL;
SDL_Surface *background = NULL;

SDL_Renderer *renderer = NULL;

Vector cameraCenter = {-0.75, 1.25, 8.0};

int startSDL() {
    int result = 0;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Failed to start SDL2:\n%s\n\n", SDL_GetError());
        result = 1;
    } else {
        window = SDL_CreateWindow("Global Illumination",
            SDL_WINDOWPOS_UNDEFINED, // window x
            SDL_WINDOWPOS_UNDEFINED, // window y
            WINDOW_WIDTH, WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN); // flags

        if(window == NULL) {
            fprintf(stderr, "Failed to create window:\n%s\n\n", SDL_GetError());
            result = 1;
        } else {
            windowSurface = SDL_GetWindowSurface(window);
        }
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    return result;
}

void quit() {
    SDL_DestroyWindow(window);
    SDL_FreeSurface(background);
    window = NULL;

    SDL_Quit();
}

void draw(SolidBucket objects) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for(int i = 0; i < WINDOW_HEIGHT; i++) {
        for(int j = 0; j < WINDOW_WIDTH; j++) {
            SDL_RenderDrawPoint(renderer, j, i);
        }
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if(startSDL()) {
        fprintf(stderr, "Failed to initialize.  Exiting.\n");
    } else {
        //draw rectangle
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        SDL_RenderFillRect(renderer, &frame);
        SDL_RenderPresent(renderer);

        draw(NULL);
        SDL_Delay(2000);
    }
    quit();
    return 0;
}
