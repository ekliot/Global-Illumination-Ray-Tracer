/**
 *
 */

#if defined(_WIN32) || defined(_WIN64)
#include "stdafx.h"
#include <windows.h>
#endif

#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include <vector>
#include <math.h>

#include <glm/glm.hpp>

using namespace glm;
#include "Ray.h"


Ray::Ray( vec3* start, vec3* dir ) : origin(start), direction(dir) {
    normalizeRay();
}

void Ray::normalizeRay(){
    double magnitude = sqrt(this->direction->x * this->direction->x
        + this->direction->y*this->direction->y
        + this->direction->z*this->direction->z);
    *(this->direction) = vec3(this->direction->x / magnitude, this->direction->y / magnitude, this->direction->z / magnitude);

}

Ray* Ray::reflectRay(vec3* normal){
  vec3 newOrigin =  vec3(*this->origin);
  vec3 newDirectiontemp = reflect(*(this->direction), *(normal));

  return new Ray(&newOrigin,&newDirectiontemp);
}
