#ifndef _SOLID_H
#define _SOLID_H

#include <SDL2/SDL.h>
#include "Vector.h"
#include "Ray.h"
#include "Figure.h"

typedef struct st_reflection {
    int hit;
    Vector intersect;
    SDL_Color color;
} Reflection;

typedef struct st_solid {
    Figure figure;
    Reflection (*reflect)(Ray, Figure);
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

SolidBucket solidBucketLast(SolidBucket bucket);

SolidBucket solidBucketPush(SolidBucket bucket, Solid solid);

int solidBucketLength(SolidBucket bucket);

Solid solidBucketPop(SolidBucket *bucket);

void SolidBucketDestroy(SolidBucket bucket);

SolidBucket solidBucketFilterInside(SolidBucket bucket, AABB inside);

SolidBucket solidBucketAppend(SolidBucket a, SolidBucket b);

SolidBucket solidBucketSort(SolidBucket bucket);

#endif
