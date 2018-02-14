#ifndef _VERTEX_H_
#define _VERTEX_H_

typedef struct Vector {
    double x;
    double y;
    double z;
} Vector;

double vectorMagnitude(Vector v);

Vector vectorNormalize(Vector v);

#endif
