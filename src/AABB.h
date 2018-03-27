#ifndef _AABB_H_
#define _AABB_H_

#include "Ray.h"

class AABB {

        float x_max;
        float y_max;
        float z_max;
        float x_min;
        float y_min;
        float z_min;

    public:

        AABB(float _x_max, float _y_max, float _z_max, float _x_min, float _y_min, float z__min);
        AABB(vec3 max, vec3 min);
        bool intersectAABB(AABB* aabb);
        float intersectRay(Ray* ray);
        vec3 getMax();
        vec3 getMin();

};

#endif
