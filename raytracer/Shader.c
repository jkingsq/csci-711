#include "Vector.h"
#include "Ray.h"
#include "Figure.h"
#include "Solid.h"
#include "Reflect.h"

Reflection shaderSphereDefault(Ray ray, Figure fig) {
    Vector intersect = raySphereIntersect(ray, fig.sphere);
    if(!isVector(intersect)) {
        return reflectionNone;
    }
    Reflection result;
    Vector normal = vectorDiff(intersect, fig.sphere.c);
    result.color.r = fabs(normal.x)*255;
    result.color.g = fabs(normal.y)*255;
    result.color.b = fmax(0, normal.z)*255;

    result.intersect = intersect;

    return result;
}

Reflection shaderTriangleTile(Ray ray, Figure fig) {
    const double tiles = 11.0;
    SDL_Color color1 = {255,   0,   0, 255};
    SDL_Color color2 = {255, 255,   0, 255};
    Reflection result;

    Vector e1 = vectorDiff(fig.triangle.points[1], fig.triangle.points[0]);
    Vector e2 = vectorDiff(fig.triangle.points[2], fig.triangle.points[0]);
    Vector normal = vectorCross(e1, e2);

    Vector tuv = rayTriangleTUV(ray, fig.triangle);
    if(!isVector(tuv)){
        return reflectionNone;
    }

    double tile = floor(fmod(tiles * (tuv.y + tuv.z), 2.0));
    if(tile == 0.0) {
        result.color = color1;
    } else {
        result.color = color2;
    }

    result.intersect = vectorSum(ray.point, vectorScale(tuv.x, ray.direction));
    return result;
}
