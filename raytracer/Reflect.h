#ifndef _REFLECT_H
#define _REFLECT_H

#include "Figure.h"
#include "Solid.h"
#include "Ray.h"

extern const Reflection reflectionNone;

Reflection getReflection(Ray ray, SolidBucket objects, int recur);

SDL_Color averageReflections(Reflection *reflections, int count);

Vector raySphereIntersect(Ray ray, Sphere sphere);

Vector rayTriangleIntersect(Ray ray, Triangle triangle);

Vector rayTriangleTUV(Ray ray, Triangle triangle);

#endif
