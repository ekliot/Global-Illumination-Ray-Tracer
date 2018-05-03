/**
 *
 */

#ifndef _INTERSECTDATA_H_
#define _INTERSECTDATA_H_

#include <vector>

#include <glm/vec3.hpp>

#include "Light.h"

using namespace glm;

typedef struct st_IntersectData {
    vec3* position;
    vec3* normal;
    vec3* incoming;
    std::vector<Light> lights;
    vec3* ambient;
    vec3* obj_color;
} IntersectData;

#endif
