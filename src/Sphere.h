#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <glm/vec3.hpp>
using glm::vec3;

#include "Object.h"
#include "Material.h"

class Sphere : public Object {
    vec3* point;
    float* radius;
    public:
        Sphere( vec3* point, float* radius, Material* mat );
        float intersection( Ray* ray );
        bool solveQuadratic( const float &a, const float &b, const float &c, float &x0, float &x1 );
        void transform( mat4 mat );
        vec3 extractScale( const mat4 &m );
};

#endif
