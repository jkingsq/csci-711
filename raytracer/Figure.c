#include <math.h>

#include "Vector.h"
#include "Ray.h"
#include "Figure.h"

int aabbIntersect(AABB a, AABB b) {
    if(a.minX > b.maxX || a.maxX > b.minX)
        return 0;
    if(a.minX > b.maxY || a.maxY > b.minY)
        return 0;
    if(a.minZ > b.maxZ || a.maxZ > b.minZ)
        return 0;
    return 1;
}

Figure makeSphere(Vector center, double radius) {
    Figure result;
    result.sphere.c = center;
    result.sphere.r = radius;

    Vector min = center, max = center;
    min.x -= radius;
    min.y -= radius;
    min.z -= radius;

    max.x += radius;
    max.y += radius;
    max.z += radius;

    result.aabb.minX = min.x;
    result.aabb.minY = min.y;
    result.aabb.minZ = min.z;

    result.aabb.maxX = max.x;
    result.aabb.maxY = max.y;
    result.aabb.maxZ = max.z;

    return result;
}

Figure makeTriangle(Vector a, Vector b, Vector c) {
    Figure result;
    result.triangle.points[0] = a;
    result.triangle.points[1] = b;
    result.triangle.points[2] = c;

    result.aabb.minX = fmin(a.x, fmin(b.x, c.x));
    result.aabb.minY = fmin(a.y, fmin(b.y, c.y));
    result.aabb.minZ = fmin(a.z, fmin(b.z, c.z));

    result.aabb.maxX = fmax(a.x, fmax(b.x, c.x));
    result.aabb.maxY = fmax(a.y, fmax(b.y, c.y));
    result.aabb.maxZ = fmax(a.z, fmax(b.z, c.z));

    return result;
}
