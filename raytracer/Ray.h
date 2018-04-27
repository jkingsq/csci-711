#ifndef _RAY_H
#define _RAY_H

#include "Vector.h"

typedef struct st_ray {
    Vector point, direction;
} Ray;

Ray rayNudge(Ray r);

Vector rayTravel(Ray r, double d);

Ray rayCircleSample(Ray r, Vector lookat, double radius, double theta,
        double focalLength);

#endif
