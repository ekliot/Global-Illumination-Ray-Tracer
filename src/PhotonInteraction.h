#ifndef _PHOTON_INTERACTION_H_
#define _PHOTON_INTERACTION_H_

#include <glm/vec3.hpp>
#include "Photon.h"
using namespace glm;

typedef struct st_Photon_Interaction {
    Photon* photon;
    vec3 color;
    vec3 inc_direction;
} PhotonInteraction;

#endif
