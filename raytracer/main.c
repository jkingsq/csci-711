#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#include "Vector.h"
#include "Matrix.h"
#include "Ray.h"
#include "Figure.h"
#include "Solid.h"
#include "Reflect.h"
#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
SDL_Color black = {0, 0, 0, 255};

SDL_Rect frame = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

SDL_Window *window = NULL;

SDL_Surface *windowSurface = NULL;

SDL_Renderer *renderer = NULL;

Vector cameraCenter = {-0.75, 1.25, 8.0};
double cameraFOV = 45.0;

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

SolidBucket makeObjects() {
    SolidBucket result = NULL;

    Solid sphere1, sphere2;

    Vector center1 = {-0.75, 1.25, -5.0};
    Vector center2 = { 0.00, 0.75, -3.5};

    sphere1.figure = makeSphere(center1, 0.5);
    sphere1.reflect = &shaderSphereDefault;

    sphere2.figure = makeSphere(center2, 0.5);
    sphere2.reflect = &shaderSphereDefault;

    solidBucketPush(result, sphere2);
    solidBucketPush(result, sphere1);
    
    return result;
}

void quit() {
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

Ray makeRay(int x, int y) {
    Ray result;
    result.point = cameraCenter;
    double tx = (double)x / (double)WINDOW_WIDTH;
    double ty = (double)y / (double)WINDOW_HEIGHT;
    double aspect = (double)WINDOW_WIDTH / (double)WINDOW_HEIGHT;

    double planeWidth = sin(cameraFOV*aspect);
    double planeHeight = sin(cameraFOV);
    Vector planeX = vectorScale(planeWidth, unitX);
    Vector planeY = vectorScale(planeHeight, unitY);

    Vector planeCorner = vectorScale(-1.0, unitZ);
    planeCorner = vectorDiff(planeCorner, vectorScale(0.5, planeX));
    planeCorner = vectorSum(planeCorner, vectorScale(0.5, planeY));

    result.direction = vectorSum(planeCorner, vectorScale(tx, planeX));
    result.direction = vectorDiff(result.direction, vectorScale(ty, planeY));
    result.direction = vectorNormalize(result.direction);

    return result;
}

void draw(SolidBucket objects) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for(int i = 0; i < WINDOW_HEIGHT; i++) {
        for(int j = 0; j < WINDOW_WIDTH; j++) {
            Ray d = makeRay(j, i);
            SDL_Color color = rayCollision(d, NULL);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
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
