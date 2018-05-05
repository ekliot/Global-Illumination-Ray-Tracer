#ifndef _PHOTON_H_
#define _PHOTON_H_

#include <glm/vec3.hpp>

using namespace glm;

typedef struct st_Photon {
    vec3 position;
    vec3 power;
    char phi, theta;
    short flag;
    float distance;  // used for min heap
} Photon;

#endif
