#include <Vector.h>

double vectorMagnitude(Vector v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

Vector vectorNormalize(Vector v) {
    double divisor = vectorMagnitude(v);
    Vector result = v;
    result.x /= divisor;
    result.y /= divisor;
    result.z /= divisor;
    return result
}
