#ifndef _FIGURE_H
#define _FIGURE_H

#include "Vector.h"

typedef struct st_aabb {
    double minX, maxX, minY, maxY, minZ, maxZ;
} AABB;

typedef struct st_sphere {
    AABB boundingBox;
    Vector c;
    double r;
} Sphere;

typedef struct st_triangle {
    AABB boundingBox;
    Vector points[3];
} Triangle;

typedef union u_figure {
    AABB aabb;
    Sphere sphere;
    Triangle triangle;
} Figure;

int aabbIntersect(AABB a, AABB b);

Figure makeSphere(Vector c, double r);

Figure makeTriangle(Vector a, Vector b, Vector c);

#endif
