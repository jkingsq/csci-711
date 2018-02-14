#ifndef _RAY_H
#define _RAY_H

#include <SDL2/SDL.h>
#include "Vector.h"

typedef struct st_ray {
    Vector point, direction;
} Ray;

SDL_Color trace(Ray ray);

#endif
