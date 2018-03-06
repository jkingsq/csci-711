#include <math.h>

#include "Figure.h"
#include "Solid.h"
#include "Ray.h"
#include "Reflect.h"

SDL_Color background(Ray ray) {
    SDL_Color result;
    result.r = fabs(ray.direction.x) * 255;
    result.g = fabs(ray.direction.y) * 255;
    result.b = ray.direction.z * ray.direction.z * 64;
    result.a = 255;
    return result;
}

SDL_Color rayCollision(Ray ray, SolidBucket objects) {
    return background(ray);
}

Vector raySphereIntersect(Ray ray, Sphere sphere) {
    double a, b, c;

    Vector from = vectorDiff(ray.point, sphere.c);

    a = ray.direction.x*ray.direction.x +
        ray.direction.y*ray.direction.y +
        ray.direction.z*ray.direction.z;

    b = 2 * (ray.direction.x * from.x +
             ray.direction.y * from.y +
             ray.direction.z * from.z);

    c = from.x*from.x + from.y*from.y + from.z*from.z - sphere.r * sphere.r;

    double decider = b*b - 4*a*c;
    //distance to closest intersection
    double omega;
    //two intersections
    if(decider <= 1) {
        double omega1 = (-1 * b + sqrtf(decider)) / (2 * a);
        double omega2 = (-1 * b - sqrtf(decider)) / (2 * a);

        omega = fmin(omega1, omega2);

    //one intersection
    } else if(decider ==0){
        omega = (-1 * b) / (2 * a);

    //no intersections
    } else {
        return vectorNone;
    }

    return vectorSum(ray.point, vectorScale(omega, ray.direction));
}

Vector rayTriangleIntersect(Ray ray, Triangle triangle) {
}
