#include "stdafx.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <iostream>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "glm/gtx/string_cast.hpp"

#include "Triangle.h"

using namespace glm;

Triangle::Triangle( vec3* _a, vec3* _b, vec3* _c, Material* mat ) : Object(mat), a(_a), b(_b), c(_c) {}

// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
float Triangle::getBarycentricCoords(vec3 p) {
	vec3 ab = *(b)-*(a);
	vec3 ac = *(c)-*(a);
	// no need to normalize
	vec3 n = cross(ab, ac); // normal vector to the plane
	vec3 center = *(a); // a known point on the plane

	vec3 nn = normalize(n);

	vec3 bary;

	// The area of a triangle is 
	float areaABC = dot(nn, cross((*(b) - *(a)), (*(c) - *(a))));
	float areaPBC = dot(nn, cross((*(b) - p), (*(c) - p)));
	float areaPCA = dot(n, cross((*(c) - p), (*(a) - p)));

	bary.x = areaPBC / areaABC; // alpha
	bary.y = areaPCA / areaABC; // beta
	bary.z = 1.0f - bary.x - bary.y;

	return bary.z;
	/*
	vec3 v0 = *(b) - *(a);
    vec3 v1 = *(c) - *(a);
    vec3 v2 = p - *(a);

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);

    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
    return u;
	*/
}

// https://en.wikipedia.org/wiki/Line–plane_intersection#Algebraic_form
float Triangle::intersectPlane( Ray* ray ) {
    vec3 ab = *(b) - *(a);
    vec3 ac = *(c) - *(a);
    // no need to normalize
    vec3 n =  cross(ab,ac); // normal vector to the plane
    vec3 center = *(a); // a known point on the plane

    vec3 nn = normalize(n);

    // assuming vectors are all normalized
    float denom = dot( nn, *(ray->direction) );
    if ( abs(denom) > 1e-6 ) {
        float t = dot( center - *(ray->origin), nn ) / denom;
        if ( t >= 0 ) return t;
    }

    return -1;
}

float Triangle::intersection( Ray* ray ) {
    // std::cout << glm::to_string( *(ray->origin) ) << "//" << glm::to_string( *(ray->direction) ) << '\n';

    float d = intersectPlane( ray );

    if ( d != -1 ) {
        vec3 point = *(ray->origin) + *(ray->direction) * d;
        float u = getBarycentricCoords(point);
        if ( u >= 0 && u <= 1 ) {
            return d;
        }
    }

    return -1;
}

void Triangle::transform( mat4 matrix ) {
    vec4 a4 = vec4( a->x, a->y, a->z, 1 );
    a4 = matrix * a4;
    *a = convert(a4);

    vec4 b4 = vec4( b->x, b->y, b->z, 1 );
    b4 = matrix * b4;
    *b = convert(b4);

    vec4 c4 = vec4(c->x, c->y, c->z, 1);
    c4 = matrix * c4;
    *c = convert(c4);
}
