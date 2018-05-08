#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
double cameraFOV = M_PI_4;

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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return result;
}

SolidBucket defaultScene() {
    SolidBucket result = NULL;

    // Make spheres
    Solid sphere1, sphere2;

    Vector center1 = {-0.75, 1.25, 5.0};
    Vector center2 = { 0.00, 0.75, 3.5};

    sphere1.figure = makeSphere(center1, 0.50);
    sphere1.reflect = &shaderSphereGlass;

    sphere2.figure = makeSphere(center2, 0.5);
    sphere2.reflect = &shaderSphereMirror;

    // Make tile floor
    Solid tri1, tri2;

    // Corners of the quad
    Vector ll = {-2.5, 0.0,  5.0};
    Vector lr = { 2.5, 0.0,  5.0};
    Vector ul = {-2.5, 0.0, -5.0};
    Vector ur = { 2.5, 0.0, -5.0};

    tri1.figure = makeTriangle(ll, ur, ul);
    tri2.figure = makeTriangle(ur, ll, lr);

    tri1.reflect = &shaderTilePhong;
    tri2.reflect = &shaderTilePhong;

    result = solidBucketPush(result, tri1);
    result = solidBucketPush(result, tri2);
    result = solidBucketPush(result, sphere2);
    result = solidBucketPush(result, sphere1);

    return result;
}

SolidBucket transmitTest() {
    SolidBucket result = defaultScene();

    Solid sphere; 
    Vector center = {-0.75, 1.25, 0.0};
    sphere.figure = makeSphere(center, 0.50);
    sphere.reflect = &shaderSpherePhong;

    result = solidBucketPush(result, sphere);

    return result;
}

SolidBucket depthTest() {
    SolidBucket result = NULL;

    Vector sphereFirst = {1.5, 0.0, 5.0};
    Vector sphereI = {-0.75, 0.0, -2.0};
    Vector sphereJ = {0.0, 1.0, 0.0};

    for(double i = 0; i <= 10; i++) {
        for(double j = 0; j <= 2; j++) {
            Solid sphere;
            Vector center = sphereFirst;
            center = vectorSum(center, vectorScale(i, sphereI));
            center = vectorSum(center, vectorScale(j, sphereJ));
            sphere.figure = makeSphere(center, 0.25);
            sphere.reflect = &shaderSpherePhong;

            result = solidBucketPush(result, sphere);
        }
    }

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
    double aspect = (double)WINDOW_HEIGHT / (double)WINDOW_WIDTH;

    double planeWidth = sin(cameraFOV);
    double planeHeight = sin(cameraFOV*aspect);
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

double focalLength = 11.0;
double aperture = 0.20;
double blurCoeff = 1.5;
int dof = 0;
int focalRings = 16;
int maxRecur = 5;

void draw(SolidBucket objects) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
//  for(int i = 0; i < WINDOW_HEIGHT; i++) {
//      for(int j = 0; j < WINDOW_WIDTH; j++) {
//          Ray ray = makeRay(j, i);
//          Reflection reflection = getReflection(ray, NULL, 1);
//          SDL_Color color = reflection.color;
//          SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
//          SDL_RenderDrawPoint(renderer, j, i);
//      }
//  }

    Reflection samples[1 + dof*focalRings];
    Uint32 renderStart = SDL_GetTicks();
    for(int i = 0; i < WINDOW_HEIGHT; i++) {
        for(int j = 0; j < WINDOW_WIDTH; j++) {
            Ray ray = makeRay(j, i);
            Reflection reflection = getReflection(ray, objects, maxRecur);
            SDL_Color color = reflection.color;

            if(dof==1) {
                if(isReflection(reflection)) {
                    double dist = vectorDist(reflection.intersect, ray.point);
                    double blur = fmax(1, blurCoeff * fabs(dist - focalLength));
                    double opacity = ceil(255.0 / blur / blur);

                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, opacity);
                    SDL_Rect fillArea = {j - blur/2, i- blur/2, blur, blur};
                    SDL_RenderFillRect(renderer, &fillArea);
                }
            } else if(dof) {
                for(int k = 0; k < dof * focalRings; k++) {
                    double theta = 2.0 * M_PI * (double)(k%dof) / (double)dof +
                        (i%2 ? M_PI / (double)dof : 0);

                    double radius = (1 + k / dof) * aperture / (double)focalRings;
                    Vector lookat = vectorScale(-1, unitZ);
                    Ray r = rayCircleSample(ray, lookat, radius, theta, focalLength);
                    samples[k] = getReflection(r, objects, maxRecur);
                }
                samples[dof * focalRings + 1] = reflection;

                color = averageReflections(samples, dof * focalRings + 1);

                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_RenderDrawPoint(renderer, j, i);
            } else if(!dof) {
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_RenderDrawPoint(renderer, j, i);
            }
            Uint32 t = SDL_GetTicks();
            if(t-renderStart >= 10000) {
                renderStart = t;
                double progress = i * WINDOW_WIDTH + j;
                double total = WINDOW_WIDTH * WINDOW_HEIGHT;
                printf("%f%\n", 100.0 * progress / total);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void triangleTest(SolidBucket objects) {

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
        printf("Rendered empty scene\n");

        sceneObjects = defaultScene();

        draw(sceneObjects);
        printf("Finished rendering.\nPress s to save a screenshot.\a\n");
        SDL_Event event;

        //used for timestamping screenshots
        time_t curtime;
        struct tm *loctime;
        int fnLength = 256;
        char filename[fnLength];
        filename[0] = '\0';

        //screenshot surface
        SDL_Surface *screenshot = SDL_CreateRGBSurface(0,
            WINDOW_WIDTH, WINDOW_HEIGHT, 32,
            0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

        while(SDL_WaitEvent(&event)){
            if(event.type == SDL_MOUSEBUTTONDOWN){
                break;
            } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_s) {
                SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888,
                    screenshot->pixels, screenshot->pitch);
                curtime = time(NULL);
                loctime = localtime(&curtime);
                strftime(filename, fnLength, "%b%d%H%M%S.bmp", loctime);
                printf("Saved screenshot: %s\n", filename);

                SDL_SaveBMP(screenshot, filename);
            } else {
                SDL_RenderPresent(renderer);
                SDL_RenderPresent(renderer);
            }
        }
    }
    quit();
    return 0;
}
