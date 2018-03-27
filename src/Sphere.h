#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <glm/vec3.hpp>
using glm::vec3;

#include "Object.h"

class Sphere : public Object {

    vec3* center;

    float radius;

    bool solve_quadratic( const float &a, const float &b, const float &c, float &x0, float &x1 );

    vec3 extract_scale( const mat4 &m );

    vec3 get_normal( Ray* ray, float distance );

    void scale_radius( mat4 tmat );

    public:

        Sphere( vec3* center, float radius, IlluminationModel* _imodel, Material* _mat );

        float intersection( Ray* ray );

        void transform( mat4 mat );

        AABB* getAABB();

};

#endif
