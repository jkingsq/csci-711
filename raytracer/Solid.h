#ifndef _SOLID_H
#define _SOLID_H

#include <SDL2/SDL.h>
#include "Vector.h"

typedef struct st_aabb {
    double minX, maxX, minY, maxY, minZ, maxZ;
} AABB;

typedef struct st_reflection {
    int miss;
    Vector intersection;
    SDL_Color color;
} Reflection;

typedef struct st_solid {
    AABB boundingBox;
    Reflection (*reflect)(Vector);
    Vector points[3];
} Solid;

typedef struct st_solidbucket {
    Solid solid;
    struct st_solidbucket *next;
} *SolidBucket;

typedef struct st_kdtree {
    SolidBucket bucket;
    AABB boundingBox;
    struct st_kdtree *left, *right;
} *KDTree;

#endif
