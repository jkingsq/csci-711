#ifndef _SHADER_H
#define _SHADER_H

#include "Ray.h"
#include "Figure.h"
#include "Solid.h"

Reflection shaderSphereDefault(Ray ray, Figure fig, int recur);

Reflection shaderSpherePhong(Ray ray, Figure fig, int recur);

Reflection shaderTriangleTile(Ray ray, Figure fig, int recur);

Reflection shaderTilePhong(Ray ray, Figure fig, int recur);

Reflection shaderTileShadow(Ray ray, Figure fig, int recur);

Reflection shaderSphereMirror(Ray ray, Figure fig, int recur);

#endif
