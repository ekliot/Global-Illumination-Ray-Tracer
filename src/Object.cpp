/**
 *
 */

#include <iostream>

#include <glm/matrix.hpp>
#include "glm/gtx/string_cast.hpp"

#include "Object.h"
#include "IntersectData.h"
#include "Material.h"

using namespace glm;

Object::Object( IlluminationModel* _imodel, Material* _mat) : imodel(_imodel), material(_mat) {}

Object::~Object() {
    delete imodel;
    delete material;
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

    vec3 o_col = material->get_color( 0.0f, 0.0f );
    // vec3 o_col = vec3( 1.0f, 0.0f, 0.0f ); // TODO this is temporary
    data.obj_color = &o_col;

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
vec2 Object::get_uv( vec3* point ){
    return vec2( 0.0f );
}
