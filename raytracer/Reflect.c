#include <math.h>

#include "Figure.h"
#include "Solid.h"
#include "Ray.h"
#include "Reflect.h"

const Reflection reflectionNone = {
    VECTOR_NONE, // intersect
    {0, 0, 0, 0}   // color
};

extern SolidBucket sceneObjects = NULL;

SDL_Color background(Ray ray) {
    SDL_Color result;
    result.r = 255 - fabs(ray.direction.x) * 255;
    result.g = 255 - fabs(ray.direction.y) * 255;
    result.b = 255 - ray.direction.z * ray.direction.z * 64;
    result.a = 255;
    return result;
}

Reflection testSolid(Ray ray, Solid solid, int recur) {
    return (solid.reflect)(ray, solid.figure, recur);
}

Reflection getReflection(Ray ray, SolidBucket objects, int recur) {
    Reflection result = reflectionNone;
    SolidBucket object = objects;
    double distance = INFINITY;
    while(object != NULL) {
        Reflection temp = testSolid(ray, object->solid, recur);
        if(isReflection(temp)) {
            double tempDist =
                vectorMagnitude(vectorDiff(temp.intersect, ray.point));
            if(tempDist < distance) {
                distance = tempDist;
                result = temp;
            }
        }
        object = object->next;
    }
    if(!isReflection(result) || distance == INFINITY)
        result.color = background(ray);
    return result;
}

SDL_Color averageReflections(Reflection *reflections, int count) {
    double r = 0, g = 0, b = 0;
    for(int i = 0; i < count; i++) {
        Reflection ref = reflections[i];
//      r+= pow((double)ref.color.r, 2.0)/(double)count;
//      g+= pow((double)ref.color.g, 2.0)/(double)count;
//      b+= pow((double)ref.color.b, 2.0)/(double)count;
        r+=(pow((double)ref.color.r, 2))/(double)count;
        g+=(pow((double)ref.color.g, 2))/(double)count;
        b+=(pow((double)ref.color.b, 2))/(double)count;
    }
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);

    SDL_Color result = {floor(r), floor(g), floor(b)};
    return result;
}

Vector raySphereIntersect(Ray ray, Sphere sphere) {
    double a, b, c;

    Vector from = vectorDiff(ray.point, sphere.c);

    a = ray.direction.x*ray.direction.x +
        ray.direction.y*ray.direction.y +
        ray.direction.z*ray.direction.z;

    b = 2 * (ray.direction.x * from.x +
             ray.direction.y * from.y +
             ray.direction.z * from.z);

    c = from.x*from.x + from.y*from.y + from.z*from.z - sphere.r * sphere.r;

    double decider = b*b - 4*a*c;
    //distance to closest intersection
    double omega;
    //two intersections
    if(decider <= 1 && decider >= 0) {
        double omega1 = (-1 * b + sqrtf(decider)) / (2 * a);
        double omega2 = (-1 * b - sqrtf(decider)) / (2 * a);

        omega = fmin(omega1, omega2);

        if(omega <= 0)
            omega = fmax(omega1, omega2);

        if(omega <= 0)
            return vectorNone;

    //one intersection
//  } else if(decider ==0){
//      omega = (-1 * b) / (2 * a);

    //no intersections
    } else {
        return vectorNone;
    }

    return vectorSum(ray.point, vectorScale(omega, ray.direction));
}

Vector rayTriangleIntersect(Ray ray, Triangle triangle) {
    Vector tuv = rayTriangleTUV(ray, triangle);

    if(isVector(tuv))
        return vectorSum(ray.point, vectorScale(tuv.x, ray.direction));
    else
        return vectorNone;
}

Vector rayTriangleTUV(Ray ray, Triangle triangle) {
    Vector e1 = vectorDiff(triangle.points[1], triangle.points[0]);
    Vector e2 = vectorDiff(triangle.points[2], triangle.points[0]);

    Vector T = vectorDiff(ray.point, triangle.points[0]);
    Vector P = vectorCross(ray.direction, e2);
    Vector Q = vectorCross(T, e1);

    double scale = vectorDot(e1, P);
    //check if ray is parallel to the triangle's plane
    if(scale == 0.0) {
        return vectorNone;
    }

    double u = vectorDot(P, T) / scale;
    double v = vectorDot(Q, ray.direction) / scale;

    //check if ray intersects with plane outside triangle bounds
    if(u < 0.0 || v < 0.0 || u+v > 1) {
        return vectorNone;
    }

    double t = vectorDot(Q, e2) / scale;

    //check if ray intersects with plane behind point of origin
    if(t < 0.0) {
        return vectorNone;
    }

    Vector result = {t, u, v};
    return result;
}
