/**
 *
 */

#include "Light.h"

Light::Light( vec3 _pos, vec3 _col ) : pos( _pos ), color( _col ) {}

Light::~Light() {}

vec3 Light::get_pos() { return vec3( pos ); }

vec3 Light::get_color() { return vec3( color ); }
