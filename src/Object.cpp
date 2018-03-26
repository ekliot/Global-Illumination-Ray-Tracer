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

    vec3 normal   = get_normal( ray, distance );
    data.normal   = &normal;

    data.incoming = ray->direction;

    data.lights   = lights;

    data.ambient  = ambient;

    return imodel->intersect( data );
}

vec3 Object::convert( vec4* vec ) {
    return vec3( vec->x / vec->w, vec->y / vec->w, vec->z / vec->w );
}

float Object::minHelper(float oldValue, float newValue)
{
    if(newValue < oldValue)
        return newValue;
    return oldValue;
}

float Object::maxHelper(float oldValue, float newValue)
{
    if(newValue > oldValue)
        return newValue;
    return oldValue;
}

vec3 Object::minVecHelper(vec3 oldValue, vec3 newValue)
{
    return vec3(
        minHelper(oldValue.x, newValue.x),
        minHelper(oldValue.y, newValue.y),
        minHelper(oldValue.z, newValue.z)
    );
}

vec3 Object::maxVecHelper(vec3 oldValue, vec3 newValue)
{
    return vec3(
        maxHelper(oldValue.x, newValue.x),
        maxHelper(oldValue.y, newValue.y),
        maxHelper(oldValue.z, newValue.z)
    );
}
