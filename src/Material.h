#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <glm/vec4.hpp>

#include "IlluminationModel.h"

using namespace glm;

typedef struct st_Material {
    vec4 color;
    IlluminationModel* imodel;
} Material;

#endif
