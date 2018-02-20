#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>


using namespace glm;

typedef struct st_Light {
    vec4 color;
    vec3 position;
} Light;

#endif
