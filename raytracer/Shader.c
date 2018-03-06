#include "Vector.h"
#include "Ray.h"
#include "Figure.h"
#include "Solid.h"
#include "Reflect.h"

Reflection shaderSphereDefault(Ray ray, Figure fig) {
    Reflection result;
    Vector intersect = raySphereIntersect(ray, fig.sphere);
    if(!isVector(intersect)) {
        return reflectionNone;
    }
    Vector normal = vectorNormalize(vectorDiff(intersect, fig.sphere.c));
    result.color.r = fabs(normal.x*255);
    result.color.g = fabs(normal.y*255);
    result.color.b = fabs(normal.z*255);

    result.intersect = intersect;

    return result;
}

Reflection shaderTriangleDefault(Ray ray, Figure fig) {

}
