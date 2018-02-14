#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <climits>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "glm/gtx/string_cast.hpp"

#include "Triangle.h"

using namespace glm;

Triangle::Triangle( vec3* _a, vec3* _b, vec3* _c, Material* mat ) : Object(mat), a(_a), b(_b), c(_c) {}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
float Triangle::intersection( Ray* ray ) {
    const float EPS = 0.0000001;

    vec3 edge1 = *b - *a;
    vec3 edge2 = *c - *a;

    vec3 p = cross( *(ray->direction), edge2 );
    float determinant = dot( edge1, p );

    if (abs(determinant) < EPS ) return INT_MAX;

    vec3 s, q;
    float u, v, t;

    float det_inv = 1/determinant;

    s = *(ray->origin) - *a;
    u = det_inv * ( dot( s, p ) );
    if ( u < 0.0 || u > 1.0 ) return INT_MAX;

    q = cross( s, edge1 );
    v = det_inv * dot( *(ray->direction), q );
    if ( v < 0.0 || u + v > 1.0 ) return INT_MAX;

    t = det_inv * dot( edge2, q );

    if ( t > EPS )  return t;
    else            return INT_MAX;
}

void Triangle::transform( mat4 matrix ) {
    vec4 _a = vec4( a->x, a->y, a->z, 1 );
    _a = matrix * _a;
    std::cout << "old a // " << glm::to_string( *a ) << '\n';
    *a = convert(_a);
    std::cout << "new a // " << glm::to_string( *a ) << '\n';

    vec4 _b = vec4( b->x, b->y, b->z, 1 );
    _b = matrix * _b;
    std::cout << "old b // " << glm::to_string( *b ) << '\n';
    *b = convert(_b);
    std::cout << "new b // " << glm::to_string( *b ) << '\n';

    vec4 _c = vec4(c->x, c->y, c->z, 1);
    _c = matrix * _c;
    std::cout << "old c // " << glm::to_string( *c ) << '\n';
    *c = convert(_c);
    std::cout << "new c // " << glm::to_string( *c ) << '\n' << '\n';
}
