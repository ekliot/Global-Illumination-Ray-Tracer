/**
 *
 */

#ifndef _SOLIDMATERIAL_H_
#define _SOLIDMATERIAL_H_

#include "Material.h"
#include <glm/vec3.hpp>
using glm::vec3;

class SolidMaterial : public Material {
    vec3 color;

public:
    SolidMaterial( vec3 color );
    ~SolidMaterial();
    vec3 get_color( float u, float v );
};

#endif
