#ifndef _MATRIX_H
#define _MATRIX_H

#include "Vector.h"

// 3x3 matrix for 3d transforms and translations
// Arranged in column-major order as follows:
//               _          _
//              | 0  4  8 12 |
// Mat4.entry = | 1  5  9 13 |
//              | 2  6 10 14 |
//              | 3  7 11 15 |
//              |_          _|
typedef struct st_mat4{
    double entry[16];
} Mat4;

//Returns the identity matrix
Mat4 mat4Identity();

// Retruns a transformation matrix which scales 3d vectors by the given amount
Mat4 mat4Scale(double sx, double sy, double sz);

// Returns a transformation matrix which rotates 3d vectors about an axis by the
// given angle measure in radians
Mat4 mat4RotateX(double theta);
Mat4 mat4RotateY(double theta);
Mat4 mat4RotateZ(double theta);

// Returns a transformation matrix which essentially performs 3d vector addition
// with the given vertex.
Mat4 mat4Translate(double tx, double ty, double tz);

// Returns a projection transformation based on the given dimensions
Mat4 mat4Frustum(double left, double right, double bottom, double top,
    double near, double far);
Mat4 mat4Ortho(double left, double right, double bottom, double top,
    double near, double far);

// Computes the product of two matrices.
Mat4 mat4Prod(Mat4 mA, Mat4 mB);

// Yields the first three components of the product of the given matrix and
// vector. Assumes the w component of the vector is 1
Vector mat4VectorProd(Mat4 m, Vector v);

void printMatrix(Mat4 m);

#endif
