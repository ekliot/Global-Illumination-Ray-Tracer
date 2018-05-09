/**
 *
 */

#ifndef _AABB_H_
#define _AABB_H_

#include <iostream>

#include "Ray.h"

class AABB {
    vec3 max;
    vec3 min;

  public:
    AABB( float max_x, float max_y, float max_z, float min_x, float min_y,
          float min_z );
    AABB( vec3 max, vec3 min );
    AABB( AABB* first, AABB* second );

    bool intersect_aabb( AABB* aabb );
    bool intersect_point( vec3 point );
    float intersect_ray( Ray* ray );

    vec3 get_max();
    vec3 get_min();

    void print();
};

#endif
