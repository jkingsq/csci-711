#ifndef _SHADER_H
#define _SHADER_H

#include "Ray.h"
#include "Figure.h"
#include "Solid.h"

Reflection shaderSphereDefault(Ray ray, Figure fig);

Reflection shaderTriangleTile(Ray ray, Figure fig);

#endif
