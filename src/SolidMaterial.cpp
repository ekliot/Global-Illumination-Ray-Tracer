/**
 *
 */

#include "SolidMaterial.h"

SolidMaterial::SolidMaterial( vec3 _col, float _kr, float _kd, float _ir )
    : Material( _kr, _kd, _ir ), color( _col ) {}

SolidMaterial::~SolidMaterial() {}

vec3 SolidMaterial::get_color( float u, float v ) { return color; }
