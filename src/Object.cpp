/**
 *
 */

#include <iostream>

#include <glm/matrix.hpp>
#include "glm/gtx/string_cast.hpp"

#include "Object.h"
#include "IntersectData.h"
#include "Texture.h"


using namespace glm;

Object::Object( IlluminationModel* _imodel, Texture* _texture) : imodel(_imodel), texture(_texture) {}

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

    // if( texture != NULL ) {
    //     // data.obj_color = texture->get_color(u,v);
    // } else {
    //     // data.obj_color = color;
    // }

    vec3 o_col = vec3( 1.0f, 0.0f, 0.0f ); // TODO this is temporary
    data.obj_color = &o_col;

    return imodel->intersect( data );
}

vec3 Object::convert( vec4* vec ) {
    return vec3( vec->x / vec->w, vec->y / vec->w, vec->z / vec->w );
}

vec2 Object::get_uv(vec3* point){
    return vec2(0,0);
}
