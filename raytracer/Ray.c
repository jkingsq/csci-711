#include "Ray.h"
#include <math.h>
#include <float.h>

Ray rayNudge(Ray r) {
    Ray result;
    result.direction = r.direction;
    Vector nudge = vectorScale(0.01, r.direction);
    result.point = vectorSum(r.point, nudge);
    return result;
}

Vector rayTravel(Ray r, double d) {
    return vectorSum(r.point, vectorScale(d, vectorNormalize(r.direction)));
}

Ray rayCircleSample(Ray r, Vector lookat, double radius, double theta,
        double focalLength) {
    //make two vectors orthogonal to lookat
    Vector u = lookat;
    Vector v = lookat;
    if(lookat.x) {
        u.x = lookat.y;
        u.y = lookat.x * -1;

        v.x = lookat.z;
        v.z = lookat.x * -1;
    } else if(lookat.y) {
        u.y = lookat.x;
        u.x = lookat.y * -1;

        v.y = lookat.z;
        v.z = lookat.y * -1;
    } else {
        u.z = lookat.x;
        u.x = lookat.z * -1;

        v.z = lookat.y;
        v.y = lookat.z * -1;
    }

    Ray result;

    u = vectorScale(radius * cos(theta), u);
    v = vectorScale(radius * sin(theta), v);
    Vector offset = vectorSum(u, v);

    result.point = vectorSum(r.point, offset);

    Vector focus = rayTravel(r, focalLength);

    result.direction = vectorNormalize(vectorDiff(focus, result.point));

    return result;
}
