/**
 *
 */

#include "Triangle.h"

#include <climits>
#include <iostream>

#include "AABB.h"
#include "glm/gtx/string_cast.hpp"

using namespace glm;

Triangle::Triangle( vec3 _a, vec3 _b, vec3 _c, IlluminationModel* _imodel,
                    Material* _mat )
    : Object( _imodel, _mat ), a( _a ), b( _b ), c( _c ) {}

Triangle::~Triangle() {
    // delete imodel;
    // delete material;
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
float Triangle::intersection( Ray* ray ) {
    const float EPS = 0.0000001;

    vec3 edge1 = b - a;
    vec3 edge2 = c - a;

    vec3 p = cross( *( ray->direction ), edge2 );
    // std::cout << "edge1 // " << glm::to_string( edge1 ) << '\n';
    // std::cout << "p     // " << glm::to_string( p ) << '\n';
    float determinant = dot( edge1, p );

    // TODO this should return a float!!!
    if ( abs( determinant ) < EPS ) return INT_MAX;

    vec3 s, q;
    float u, v, t;

    float det_inv = 1 / determinant;

    s = *( ray->origin ) - a;
    // std::cout << "s // " << glm::to_string( s ) << '\n';
    // std::cout << "p // " << glm::to_string( p ) << '\n';
    u = det_inv * ( dot( s, p ) );
    // TODO this should return a float!!!
    if ( u < 0.0 || u > 1.0 ) return INT_MAX;

    q = cross( s, edge1 );
    // std::cout << "ray->dir // " << glm::to_string( *( ray->direction ) )
    //           << '\n';
    // std::cout << "p        // " << glm::to_string( q ) << '\n';
    v = det_inv * dot( *( ray->direction ), q );
    // TODO this should return a float!!!
    if ( v < 0.0 || u + v > 1.0 ) return INT_MAX;

    // std::cout << "edge2 // " << glm::to_string( edge2 ) << '\n';
    // std::cout << "p     // " << glm::to_string( q ) << '\n' << endl;
    t = det_inv * dot( edge2, q );

    if ( t > EPS ) {
        // std::cout<<glm::to_string(firstVec)<<std::endl;
        // std::cout<<glm::to_string(secondVec)<<std::endl;
        // std::cout<<glm::to_string(thirdVec)<<std::endl;

        return t;
    }
    // TODO this should return a float!!!
    else
        return INT_MAX;
}

vec3 Triangle::get_normal( Ray* ray, float distance ) {
    /*
        Set Vector U to (Triangle.b minus Triangle.a)
        Set Vector V to (Triangle.c minus Triangle.a)

        Set Normal.x to (multiply U.y by V.z) minus (multiply U.z by V.y)
        Set Normal.y to (multiply U.z by V.x) minus (multiply U.x by V.z)
        Set Normal.z to (multiply U.x by V.y) minus (multiply U.y by V.x)
    */

    vec3 u = b - a;
    vec3 v = c - a;
    vec3 normal =
        vec3( ( u.y * v.z ) - ( u.z * v.y ), ( u.z * v.x ) - ( u.x * v.z ),
              ( u.x * v.y ) - ( u.y * v.x ) );
    return normalize( normal );
}

void Triangle::transform( mat4 matrix ) {
    vec4 _a = vec4( a.x, a.y, a.z, 1 );
    _a      = matrix * _a;
    a       = convert( _a );

    vec4 _b = vec4( b.x, b.y, b.z, 1 );
    _b      = matrix * _b;
    b       = convert( _b );

    vec4 _c = vec4( c.x, c.y, c.z, 1 );
    _c      = matrix * _c;
    c       = convert( _c );
}

vec3 Triangle::get_a() { return vec3( a ); }

vec3 Triangle::get_b() { return vec3( b ); }

vec3 Triangle::get_c() { return vec3( c ); }

AABB* Triangle::get_aabb() {
    vec3 vec_max = vec3( a.x, a.y, a.z );
    vec3 vec_min = vec3( a.x, a.y, a.z );

    vec_max = max_vec( vec_max, b );
    vec_max = max_vec( vec_max, c );

    vec_min = min_vec( vec_min, b );
    vec_min = min_vec( vec_min, c );

    return new AABB( vec_max.x, vec_max.y, vec_max.z, vec_min.x, vec_min.y,
                     vec_min.z );
}
// TODO actually implement world-to-obj coords
vec3 Triangle::world_to_obj_space( vec3 point ) { return point; }

// TODO actually implement uv coords
vec2 Triangle::get_uv( vec3 point ) { return vec2( 0.0f ); }
