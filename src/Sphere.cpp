#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <algorithm>
#include <cmath>
#include <iostream>

#include <glm/gtx/norm.hpp>
#include "glm/gtx/string_cast.hpp"

using namespace std;
using glm::vec3;
using glm::vec4;

#include "Sphere.h"

/**************\
 PUBLIC MEMBERS
\**************/

Sphere::Sphere( vec3* c, float* r, IlluminationModel* _imodel ) : Object(_imodel), center(c), radius(r) {}

float Sphere::intersection( Ray* ray ) {
    float t0, t1; // solutions for t if the ray intersects

    vec3 L = *(ray->origin) - *(center);
    float a = dot( *(ray->direction), *(ray->direction) );
    float b = 2 * dot( *(ray->direction), L );
    float c = dot( L, L ) - ( *(radius) * *(radius) );
    if ( !solve_quadratic( a, b, c, t0, t1 ) ) return -1;

    if ( t0 > t1 ){
        swap( t0, t1 );
    }
    if ( t0 < 0 ) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if ( t0 < 0 ) return -1; // both t0 and t1 are negative
    }

    return t0;
}

void Sphere::transform( mat4 tmat ) {
    std::cout << "old sphere center // " << glm::to_string( *center ) << '\n';

    vec4 _center = tmat * vec4( center->x, center->y, center->z, 1 );
    *(center) = convert( _center );

    // scale_radius( tmat );

    std::cout << "new sphere center // " << glm::to_string( *center ) << '\n';
}

/***************\
 PRIVATE MEMBERS
\***************/

bool Sphere::solve_quadratic( const float &a, const float &b, const float &c, float &x0, float &x1 ) {
    float discr = b * b - 4 * a * c;

    if ( discr < 0 ) {
        return false;
    } else if ( discr == 0 ) {
        x0 = x1 = -0.5 * b / a;
    } else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }

    if ( x0 > x1 ) {
        swap( x0, x1 );
    }

    return true;
}

void Sphere::scale_radius( mat4 tmat ) {
    vec3 scalesSq = extract_scale( tmat );

    float max = scalesSq.x;
    if ( scalesSq.y > max ) max = scalesSq.y;
    if ( scalesSq.z > max ) max = scalesSq.z;

    float const largestScale = std::sqrt( max );

    *radius = *(radius) * largestScale;
}

vec3 Sphere::extract_scale(const mat4 &m) {
    // length2 returns length squared i.e. v�v
    // no square root involved
    return vec3( length2( vec3( m[0] ) ),
                 length2( vec3( m[1] ) ),
                 length2( vec3( m[2] ) ) );
}
