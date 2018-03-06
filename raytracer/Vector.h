#ifndef _VERTEX_H_
#define _VERTEX_H_

typedef struct Vector {
    double x;
    double y;
    double z;
} Vector;

// unit vectors
extern const Vector unitX;
extern const Vector unitY;
extern const Vector unitZ;
extern const Vector vectorNone;
#define VECTOR_NONE {-INFINITY, -INFINITY, -INFINITY}

// usually better read as 'zero vector'
extern const Vector origin;

Vector vectorSum(Vector u, Vector v);

Vector vectorDiff(Vector u, Vector v);

Vector vectorScale(double scalar, Vector v);

Vector vectorCross(Vector u, Vector v);

double vectorMagnitude(Vector v);

double vectorDist(Vector u, Vector v);

Vector vectorNormalize(Vector v);

int vectorEqual(Vector u, Vector v);

int isVector(Vector v);

#endif
