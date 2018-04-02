
#include "SolidMaterial.h"
using glm::vec3;

/**************\
 PUBLIC MEMBERS
\**************/

SolidMaterial::SolidMaterial( vec3 _col, float _kr, float _kd ) :
    Material(_kr, _kd), color(_col) {}
SolidMaterial::~SolidMaterial() {}

vec3 SolidMaterial::get_color( float u, float v ) {
    return color;
}
