#include "Vector.h"

// unit vectors
const Vector unitX = {1.0, 0.0, 0.0};
const Vector unitY = {0.0, 1.0, 0.0};
const Vector unitZ = {0.0, 0.0, 1.0};

// usually better read as 'zero vector'
const Vector origin = {0.0, 0.0, 0.0};

double vectorMagnitude(Vector v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

Vector vectorNormalize(Vector v) {
    double divisor = vectorMagnitude(v);
    Vector result = v;
    result.x /= divisor;
    result.y /= divisor;
    result.z /= divisor;
    return result;
}

Vector vectorSum(Vector u, Vector v) {
    Vector result;
    result.x = u.x + v.x;
    result.y = u.y + v.y;
    result.z = u.z + v.z;
    return result;
}

Vector vectorDiff(Vector u, Vector v) {
    Vector result;
    result.x = u.x - v.x;
    result.y = u.y - v.y;
    result.z = u.z - v.z;
    return result;
}

Vector vectorScale(double scalar, Vector v) {
    Vector result;
    result.x = scalar * v.x;
    result.y = scalar * v.y;
    result.z = scalar * v.z;
    return result;
}

Vector vectorCross(Vector u, Vector v) {
    Vector result;
    result.x = u.y*v.z - u.z*v.y;
    result.y = u.z*v.x - u.x*v.z;
    result.z = u.x*v.y - u.y*v.x;
    return result;
}
