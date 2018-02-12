/**
 *
 */

#if defined(_WIN32) || defined(_WIN64)
#include "stdafx.h"
#include <windows.h>
#endif

#include <vector>
#include <math.h>

#include <glm/vec3.hpp>

using namespace glm;
#include "Ray.h"

Ray::Ray( vec3 start, vec3 dir ) : origin(start), direction(dir) {
    normalize();
}

void Ray::normalize(){
    double magnitude = sqrt(direction.x * direction.x
        + direction.y*direction.y
        + direction.z*direction.z);
    direction = vec3(direction.x / magnitude, direction.y / magnitude, direction.z / magnitude);
}
