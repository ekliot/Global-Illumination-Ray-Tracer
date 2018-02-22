/**
 *
 */

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

    double magnitude = sqrt( direction->x * direction->x
                           + direction->y * direction->y
                           + direction->z * direction->z);
    *(direction) = vec3(
        direction->x / magnitude,
        direction->y / magnitude,
        direction->z / magnitude
    );

}

Ray* Ray::reflectRay( vec3* normal ){
  vec3 newOrigin =  vec3( *origin );
  vec3 newDirectiontemp = reflect( *(direction), *(normal) );

  return new Ray( &newOrigin, &newDirectiontemp );
}
