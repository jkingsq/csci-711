#include <math.h>

#include "Vector.h"
#include "Ray.h"
#include "Figure.h"
#include "Solid.h"
#include "Reflect.h"

Reflection shaderSphereDefault(Ray ray, Figure fig, int recur) {
    Vector intersect = raySphereIntersect(ray, fig.sphere);
    if(!isVector(intersect)) {
        return reflectionNone;
    }
    Reflection result;
    Vector normal = vectorScale(1/fig.sphere.r,
        vectorDiff(intersect, fig.sphere.c));
    
    result.color.r = fabs(normal.x)*255;
    result.color.g = fabs(normal.y)*255;
    result.color.b = fmax(0, normal.z)*255;

    result.intersect = intersect;

    return result;
}

Vector light = {-0.75, 5.0, 8.0};

Reflection shaderSpherePhong(Ray ray, Figure fig, int recur) {
    Vector intersect = raySphereIntersect(ray, fig.sphere);
    if(!isVector(intersect)) {
        return reflectionNone;
    }
    Reflection result;
    Vector normal = vectorScale(1/fig.sphere.r,
        vectorDiff(intersect, fig.sphere.c));

    Vector vL = vectorNormalize(vectorDiff(light, intersect));

    Vector nvL = vectorScale(-1.0, vL);
    Vector vR = vectorDiff(nvL, vectorScale(2, vectorScale(vectorDot(nvL, normal), normal)));

    Vector vV = vectorScale(-1, ray.direction);

    Vector ambient = {24, 24, 24};

    Vector diffuse = {255, 255, 255};
    diffuse = vectorScale(fmax(vectorDot(vL, normal), 0), diffuse);
    diffuse = vectorScale(2, diffuse);

    Vector specular = {255, 255, 255};
    specular = vectorScale(20, specular);
    specular = vectorScale(pow(fmax(0, vectorDot(vR, vV)), 2.0), specular);

    Vector final = vectorSum(vectorSum(ambient, diffuse), specular);

    result.color.r = fmin(final.x, 255);
    result.color.g = fmin(final.y, 255);
    result.color.b = fmin(final.z, 255);

    result.intersect = intersect;

    return result;
}

Reflection shaderTriangleTile(Ray ray, Figure fig, int recur) {
    const double tiles = 10.0;
    SDL_Color color1 = {255,   0,   0, 255};
    SDL_Color color2 = {255, 255,   0, 255};
    Reflection result;

    Vector e1 = vectorDiff(fig.triangle.points[1], fig.triangle.points[0]);
    Vector e2 = vectorDiff(fig.triangle.points[2], fig.triangle.points[0]);

    Vector tuv = rayTriangleTUV(ray, fig.triangle);
    if(!isVector(tuv)){
        return reflectionNone;
    }

    double tileX = floor(fmod(tiles * (tuv.y), 2.0));
    double tileY = floor(fmod(tiles * (tuv.y+tuv.z), 2.0));
    if(tileX == 0.0 != tileY == 0.0) {
        result.color = color1;
    } else {
        result.color = color2;
    }

    result.intersect = vectorSum(ray.point, vectorScale(tuv.x, ray.direction));
    return result;
}

Reflection shaderTilePhong(Ray ray, Figure fig, int recur) {
    Reflection unlit = shaderTriangleTile(ray, fig, 0);
    if(!isReflection(unlit)) {
        return reflectionNone;
    }

    Vector intersect = unlit.intersect;

    Vector e1 = vectorDiff(fig.triangle.points[1], fig.triangle.points[0]);
    Vector e2 = vectorDiff(fig.triangle.points[2], fig.triangle.points[0]);

    Vector normal = vectorNormalize(vectorCross(e1, e2));

    Vector vL = vectorNormalize(vectorDiff(light, intersect));

    Vector nvL = vectorScale(-1.0, vL);
    Vector vR = vectorDiff(nvL, vectorScale(2, vectorScale(vectorDot(nvL, normal), normal)));

    Vector vV = vectorScale(-1, ray.direction);

    Vector unlitColor = {unlit.color.r, unlit.color.g, unlit.color.b};

    Vector ambient = vectorScale(0.3, unlitColor);

    Vector diffuse = vectorScale(0.7, unlitColor);

    if(recur) {
        Ray toLight = {intersect, vL};
        Vector between = getReflection(toLight, sceneObjects, 0).intersect;
        
        if(isVector(between) && vectorDist(intersect, between) < vectorDist(intersect, light)) {
            diffuse = vectorScale(0, diffuse);
        } else {
            diffuse = vectorScale(fmax(vectorDot(vL, normal), 0), diffuse);
        }
    }

    Vector specular = {0, 0, 0};
//  specular = vectorScale(10, specular);
//  specular = vectorScale(pow(fmax(0, vectorDot(vR, vV)), 2.0), specular);

    Vector final = vectorSum(vectorSum(ambient, diffuse), specular);

    Reflection result;

    result.color.r = fmin(final.x, 255);
    result.color.g = fmin(final.y, 255);
    result.color.b = fmin(final.z, 255);

    result.intersect = intersect;

    return result;
}

Reflection shaderTileShadow(Ray ray, Figure fig, int recur) {

    Vector tuv = rayTriangleTUV(ray, fig.triangle);
    if(!isVector(tuv)){
        return reflectionNone;
    }
    SDL_Color lit = {255, 0, 0, 255};
    SDL_Color unlit = {0, 0, 255};
    Reflection result;

    Vector intersect = vectorSum(ray.point, vectorScale(tuv.x, ray.direction));
    result.intersect = intersect;

    if(recur) {
        Ray toLight;
        toLight.point = intersect;
        toLight.direction = vectorNormalize(vectorDiff(light, intersect));

        Vector between = getReflection(toLight, sceneObjects, 0).intersect;

        if(isVector(between) && vectorDist(intersect, between) > vectorDist(intersect, light)) {
            result.color = unlit;
        } else {
            result.color = lit;
        }
    }
    return result;
}

Reflection shaderSphereMirror(Ray ray, Figure fig, int recur) {
    Vector intersect = raySphereIntersect(ray, fig.sphere);
    if(!isVector(intersect)) {
            return reflectionNone;
    }
    Vector normal = vectorScale(1/fig.sphere.r,
        vectorDiff(intersect, fig.sphere.c));

    if(recur) {
        Ray reflect;
        Vector vV = vectorScale(-1, ray.direction);
        Vector projection = vectorScale(vectorDot(vV, normal), normal);
        reflect.direction = vectorDiff(vV,
            vectorScale(2, vectorDiff(vV, projection)));
        reflect.point = vectorSum(intersect, vectorScale(0.01, reflect.direction));
        Reflection result = getReflection(reflect, sceneObjects, recur-1);
        result.intersect = intersect;
        return result;
    } else {
        SDL_Color noReflect = {0, 0, 0, 0};
        Reflection result;
        result.intersect = intersect;
        result.color = noReflect;
        return result;
    }
}
