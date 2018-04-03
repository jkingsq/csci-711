#include "Ray.h"
#include <float.h>

Ray rayNudge(Ray r) {
    Ray result;
    result.direction = r.direction;
    Vector nudge = vectorScale(0.01, r.direction);
    result.point = vectorSum(r.point, nudge);
    return result;
}
