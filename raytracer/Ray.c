#include "Ray.h"

SDL_Color trace(Ray ray) {
    ray.direction = vectorNormalize(ray.direction);
    SDL_Color result = {ray.direction.x, ray.direction.y, ray.direction.z, 255};
    return result;
}
