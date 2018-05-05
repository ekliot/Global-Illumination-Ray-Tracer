/**
 *
 */

#include <iostream>

#include "glm/gtx/string_cast.hpp"

using glm::vec3;
using glm::vec4;
using namespace std;

#include "Rectangle.h"

/**************\
 PUBLIC MEMBERS
\**************/

Rectangle::Rectangle( vec3 a, vec3 b, vec3 c, vec3 d,
                      IlluminationModel* _imodel, Material* _mat )
    : Object( _imodel, _mat ) {
    /**  A-----B
       /      /
      D-----C  **/

    // TODO these should COPY imodel and material
    tri1 = new Triangle( vec3( a ), vec3( b ), vec3( c ), imodel, material );
    tri2 = new Triangle( vec3( c ), vec3( d ), vec3( a ), imodel, material );
}

Rectangle::~Rectangle() {
    // TODO see constructor for why these are commented out
    // delete tri1;
    // delete tri2;
}

vec3 Rectangle::get_a() { return tri1->get_a(); }

vec3 Rectangle::get_b() { return tri1->get_b(); }

vec3 Rectangle::get_c() { return tri1->get_c(); }

vec3 Rectangle::get_d() { return tri2->get_b(); }

void Rectangle::transform( mat4 mat ) {
    tri1->transform( mat );
    tri2->transform( mat );
}

float Rectangle::intersection( Ray* ray ) {
    float temp = tri2->intersection( ray );
    if ( temp == INT_MAX ) {
        return tri1->intersection( ray );
    }
    return temp;
}

vec3 Rectangle::get_normal( Ray* ray, float distance ) {
    return tri1->get_normal( ray, distance );
}

AABB* Rectangle::getAABB() {
    vec3 vec_max = vec3( ul->x, ul->y, ul->z );
    vec3 vec_min = vec3( ul->x, ul->y, ul->z );

    vec_max = maxVecHelper( vec_max, *ur );
    vec_max = maxVecHelper( vec_max, *ll );
    vec_max = maxVecHelper( vec_max, *lr );

    vec_min = minVecHelper( vec_min, *ur );
    vec_min = minVecHelper( vec_min, *ll );
    vec_min = minVecHelper( vec_min, *lr );

    return new AABB( vec_max.x, vec_max.y, vec_max.z, vec_min.x, vec_min.y,
                     vec_min.z );
}
// TODO actually implement world-to-obj conversion
vec3 Rectangle::world_to_obj_space( vec3 point ) { return point; }

// TODO actually implement uv coords for real
// HACK this does not bother converting to object coordinates!!!
vec2 Rectangle::get_uv( vec3 point ) {
    float u = 0.0f;
    float v = 0.0f;

    vec3 a = get_a();
    vec3 b = get_b();
    vec3 c = get_c();
    vec3 d = get_d();

    if ( a.y == c.y && b.y == d.y ) {
        float x_min = std::min( a.x, std::min( b.x, std::min( c.x, d.x ) ) );
        float z_min = std::min( a.z, std::min( b.z, std::min( c.z, d.z ) ) );

        float x_max = std::max( a.x, std::max( b.x, std::max( c.x, d.x ) ) );
        float z_max = std::max( a.z, std::max( b.z, std::max( c.z, d.z ) ) );

        float w = x_max - x_min;
        float l = z_max - z_min;

        u = ( x_max - point.x ) / w;
        v = ( z_max - point.z ) / l;
    }

    return vec2( u, v );
}
