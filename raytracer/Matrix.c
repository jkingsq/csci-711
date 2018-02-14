// Adapted from the midterm project

#include <math.h>
#include <stdio.h>

#include "Matrix.h"
#include "Vector.h"

Mat4 mat4Identity() {
    Mat4 m;
    for(int i = 0; i < 16; i++) {
        if(i == 0 || i == 5 || i == 10 || i == 15)
            m.entry[i] = 1.0;
        else
            m.entry[i] = 0.0;
    }
    return m;
}

Mat4 mat4Scale(double sx, double sy, double sz) {
    Mat4 m = mat4Identity();
    m.entry[0] = sx;
    m.entry[5] = sy;
    m.entry[10] = sz;
    return m;
}

Mat4 mat4RotateX(double theta) {
    Mat4 m = mat4Identity();
    m.entry[5] = cos(theta);
    m.entry[6] = sin(theta);
    m.entry[9] = -1 * sin(theta);
    m.entry[10] = cos(theta);
    return m;
}

Mat4 mat4RotateY(double theta) {
    Mat4 m = mat4Identity();
    m.entry[0] = cos(theta);
    m.entry[2] = cos(theta);
    m.entry[8] = sin(theta);
    m.entry[10] = -1 * sin(theta);
    return m;
}

Mat4 mat4RotateZ(double theta) {
    Mat4 m = mat4Identity();
    m.entry[0] = cos(theta);
    m.entry[1] = sin(theta);
    m.entry[4] = -1 * sin(theta);
    m.entry[5] = cos(theta);
    return m;
}

Mat4 mat4Translate(double tx, double ty, double tz) {
    Mat4 m = mat4Identity();
    m.entry[12] = tx;
    m.entry[13] = ty;
    m.entry[14] = tz;
    return m;
}

Mat4 mat4Ortho(double left, double right, double bottom, double top,
        double near, double far) {
    Mat4 m = mat4Identity();

    // normalize x with respect to view width
    m.entry[0] = 2 / (right - left);
    m.entry[12] = -1 * (right + left) / (right - left);

    // normalize y with respect to view height
    m.entry[5]  = 2 / (top - bottom);
    m.entry[13] = -1 * (top + bottom) / (top - bottom);

    // normalize z with respect to view depth
    m.entry[10] = -2 / (far - near);
    m.entry[14] = -1 * (far + near) / (far - near);

    return m;
}

Mat4 mat4Frustum(double left, double right, double bottom, double top,
        double near, double far) {
    Mat4 m = mat4Identity();

    m.entry[0] = 2 * near / (right - left);
    m.entry[8] = (right + left) / (right - left);

    m.entry[5] = 2 * near / (top - bottom);
    m.entry[9] = (top + bottom) / (top - bottom);

    m.entry[10] = -1 * (far + near) / (far - near);
    m.entry[14] = -2 * far * near / (far - near);

    m.entry[11] = -1;
    m.entry[15] = 0;

    return m;
}

Mat4 mat4Prod(Mat4 mA, Mat4 mB) {
    Mat4 mAB;
    // i iterates on rows of the resulting matrix
    for(int i = 0; i < 4; i++) {
        // j iterates on columns of the resulting matrix
        for(int j = 0; j < 4; j++) {
            mAB.entry[i + 4*j] = 0;
            // each entry in the resulting matrix is the sum of four terms
            for(int k = 0; k < 4; k++) {
                mAB.entry[i + 4*j] +=
                    mA.entry[i + k*4] * mB.entry[k + j*4];
            }
        }
    }

    return mAB;
}

Vector mat4VectorProd(Mat4 m, Vector v) {
    Vector result;

    result.x =  m.entry[0]  * v.x;
    result.x += m.entry[4]  * v.y;
    result.x += m.entry[8]  * v.z;
    result.x += m.entry[12] *   1;

    result.y =  m.entry[1]  * v.x;
    result.y += m.entry[5]  * v.y;
    result.y += m.entry[9]  * v.z;
    result.y += m.entry[13] *   1;

    result.z =  m.entry[2]  * v.x;
    result.z += m.entry[6]  * v.y;
    result.z += m.entry[10] * v.z;
    result.z += m.entry[14] *   1;

    // Vector data type has no w component, so it is not computed
    return result;
}

void printMatrix(Mat4 m) {
    for(int i = 0; i < 4; i++) {
        printf("\n%f\t%f\t%f\t%f", m.entry[i], m.entry[i+4], m.entry[i+8],
            m.entry[i+12]);
    }
    printf("\n");
}
