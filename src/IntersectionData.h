#ifndef _INTERSECTIONDATA_H_
#define _INTERSECTIONDATA_H_

#include <glm/vec3.hpp>
#include "Light.h"

using namespace glm;

typedef struct st_IntersectionData {
    vec3 position;
    vec3 normal;
    vec3 incoming;
    std::vector<Light> lights;

} IntersectionData;

#endif
