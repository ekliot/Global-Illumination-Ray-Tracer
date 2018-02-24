/**
 *
 */

#include <iostream>

#include <glm/matrix.hpp>
#include "glm/gtx/string_cast.hpp"

#include "Object.h"
#include "IntersectData.h"

using namespace glm;

Object::Object( IlluminationModel* _imodel ) : imodel(_imodel) {}

void Object::transform( mat4 matrix ) {}

vec3 Object::get_color( Ray* ray, float distance, std::vector<Light*> lights, vec3* ambient ) {
    IntersectData data;
    *(data.position) = *(ray->origin) + *(ray->direction) * distance;
    // std::cout << "data.pos // " << glm::to_string( *(data.position) ) << '\n';
    vec3 normal   = get_normal( ray );
    data.normal   = &normal;
    // std::cout << "data.norm // " << glm::to_string( *(data.normal) ) << '\n';
    data.incoming    = ray->direction;
    // std::cout << "data.inc // " << glm::to_string( *(data.incoming) ) << '\n';
    data.lights      = lights;
    // std::cout << "data.lights // " << data.lights << '\n';
    data.ambient     = ambient;
    // std::cout << "data.amb // " << glm::to_string( *(data.ambient) ) << '\n';
    return imodel->intersect( data );
    // return vec3( 1.0f, 0.0f, 0.0f );
}

vec3 Object::convert( vec4* vec ) {
    return vec3( vec->x / vec->w, vec->y / vec->w, vec->z / vec->w );
}
