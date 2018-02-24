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

Object::~Object() {
    delete imodel;
}

void Object::transform( mat4 matrix ) {}

vec3 Object::get_color( Ray* ray, float distance, std::vector<Light*> lights, vec3* ambient ) {
    IntersectData data;

    vec3 position = *(ray->origin) + *(ray->direction) * distance;
    data.position = &position;

    vec3 normal   = get_normal( ray );
    data.normal   = &normal;

    data.incoming = ray->direction;

    data.lights   = lights;

    data.ambient  = ambient;

    return imodel->intersect( data );
}

vec3 Object::convert( vec4* vec ) {
    return vec3( vec->x / vec->w, vec->y / vec->w, vec->z / vec->w );
}
