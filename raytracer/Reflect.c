#include <math.h>

#include "Figure.h"
#include "Solid.h"
#include "Ray.h"
#include "Reflect.h"

const Reflection reflectionNone = {
    0,          // hit
    VECTOR_NONE, // intersect
    {0, 0, 0, 0}   // color
};

SDL_Color background(Ray ray) {
    SDL_Color result;
    result.r = 255 - fabs(ray.direction.x) * 255;
    result.g = 255 - fabs(ray.direction.y) * 255;
    result.b = 255 - ray.direction.z * ray.direction.z * 64;
    result.a = 255;
    return result;
}

Reflection testSolid(Ray ray, Solid solid) {
    return (solid.reflect)(ray, solid.figure);
}

Reflection getReflection(Ray ray, SolidBucket objects) {
    Reflection result = reflectionNone;
    SolidBucket object = objects;
    while(object != NULL) {
        Reflection temp = testSolid(ray, object->solid);
        if(temp.intersect.z > result.intersect.z)
            result = temp;
        object = object->next;
    }
    if(!result.hit)
        result.color = background(ray);
    return result;
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
