/**
 *
 */

#ifndef _SOLIDMATERIAL_H_
#define _SOLIDMATERIAL_H_

#include <glm/vec3.hpp>

#include "Material.h"
using glm::vec3;

class SolidMaterial : public Material {
    vec3 color;

  public:
    SolidMaterial( vec3 color, float kr = 0.0f, float kd = 0.0f,
                   float ir = 0.0f );
    ~SolidMaterial();
    vec3 get_color( float u, float v );
};

#endif
