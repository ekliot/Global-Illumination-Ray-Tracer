/**
 *
 */

#include "Object.h"

#include <iostream>

#include "IntersectData.h"
#include "glm/gtx/string_cast.hpp"

Object::Object( IlluminationModel* _imodel, Material* _mat )
    : imodel( _imodel ), material( _mat ) {}

Object::~Object() {
    if ( imodel != nullptr ) {
        delete imodel;
        imodel = nullptr;
    }
    if ( material != nullptr ) {
        delete material;
        material = nullptr;
    }
}

void Object::transform( mat4 matrix ) {}

vec3 Object::get_color( IntersectData data ) {
    vec2 uv        = get_uv( world_to_obj_space( *data.position ) );
    vec3 o_col     = material->get_color( uv.x, uv.y );
    data.obj_color = &o_col;

    // if ( uv.x > 1 || uv.x < 0 || uv.y > 1 || uv.y < 0 ) {
    //     std::cout << "ASDFASG" << '\n';
    // }

    return imodel->intersect( data );
}

vec3 Object::convert( vec4 vec ) {
    return vec3( vec.x / vec.w, vec.y / vec.w, vec.z / vec.w );
}

vec3 Object::min_vec( vec3 _a, vec3 _b ) {
    using std::min;
    return vec3( min( _a.x, _b.x ), min( _a.y, _b.y ), min( _a.z, _b.z ) );
}

vec3 Object::max_vec( vec3 _a, vec3 _b ) {
    using std::max;
    return vec3( max( _a.x, _b.x ), max( _a.y, _b.y ), max( _a.z, _b.z ) );
}

// HACK I don't know what to do here
vec3 Object::world_to_obj_space( vec3 point ) { return point; }

// HACK blech
vec2 Object::get_uv( vec3 point ) { return vec2( 0.0f ); }

Material* Object::get_material() { return material; }
IlluminationModel* Object::get_imodel() { return imodel; }
