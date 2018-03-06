#ifndef _REFLECT_H
#define _REFLECT_H

#include "Figure.h"
#include "Solid.h"
#include "Ray.h"

extern const Reflection reflectionNone;

Reflection getReflection(Ray ray, SolidBucket objects);

Vector raySphereIntersect(Ray ray, Sphere sphere);

Vector rayTriangleIntersect(Ray ray, Triangle triangle);

#endif
